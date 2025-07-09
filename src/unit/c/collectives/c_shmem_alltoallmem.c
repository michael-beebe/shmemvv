/**
 * @file c_shmem_alltoallmem.c
 *
 * @brief Unit test for shmem_alltoallmem().
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "log.h"
#include "shmemvv.h"

/**
 * Test function for shmem_alltoallmem with a given element size
 */
static bool test_alltoallmem_with_size(size_t elem_size) {
  char routine_name[64];
  snprintf(routine_name, sizeof(routine_name), "shmem_alltoallmem");
  log_routine(routine_name);

  int npes = shmem_n_pes();
  int mype = shmem_my_pe();

  /* Total bytes per PE */
  size_t total_bytes = npes * elem_size;

  void *src = shmem_malloc(total_bytes);
  void *dest = shmem_malloc(total_bytes);
  log_info("shmem_malloc'd %zu bytes @ &src = %p, %zu bytes @ &dest = %p",
           total_bytes, src, total_bytes, dest);

  /* Initialize source array - each PE puts its PE number in all elements */
  unsigned char *src_bytes = (unsigned char *)src;
  for (int i = 0; i < npes; i++) {
    for (size_t j = 0; j < elem_size; j++) {
      /* Each PE puts its PE number in all source elements */
      src_bytes[i * elem_size + j] = (unsigned char)(mype % 256);
    }
  }
  log_info("set %p..%p with PE number %d", src, (char *)src + total_bytes - 1,
           mype);

  /* Clear destination before test */
  memset(dest, 0xFF, total_bytes);

  /* Ensure all PEs are ready before starting alltoallmem */
  shmem_barrier_all();

  /* Execute alltoallmem */
  log_info("executing shmem_alltoallmem: dest = %p, src = %p, elem_size = %zu",
           dest, src, elem_size);
  shmem_alltoallmem(SHMEM_TEAM_WORLD, dest, src, elem_size);

  /* Ensure all PEs complete the alltoallmem before validation */
  shmem_barrier_all();

  /* Validate results */
  log_info("validating result...");
  bool success = true;
  unsigned char *dest_bytes = (unsigned char *)dest;
  for (int i = 0; i < npes; i++) {
    for (size_t j = 0; j < elem_size; j++) {
      /* After alltoallmem, dest[i] should contain the value that PE i */
      /* had in src[mype], which is just i (since each PE puts its PE# */
      /* in all source elements) */
      unsigned char expected = (unsigned char)(i % 256);
      if (dest_bytes[i * elem_size + j] != expected) {
        log_info("PE %d index %d.%zu of dest (%p) failed. expected %d, got %d",
                 mype, i, j, &dest_bytes[i * elem_size + j], expected,
                 dest_bytes[i * elem_size + j]);
        success = false;
        break;
      }
    }
    if (!success)
      break;
  }

  if (success)
    log_info(
        "shmem_alltoallmem with element size %zu produced expected result.",
        elem_size);
  else
    log_fail("at least one value was unexpected in result of shmem_alltoallmem "
             "with element size %zu",
             elem_size);

  shmem_free(src);
  shmem_free(dest);

  return success;
}

int main(int argc, char *argv[]) {
  shmem_init();
  log_init(__FILE__);

  int result = true;
  int rc = EXIT_SUCCESS;

  /* Test shmem_alltoallmem with various element sizes */
  result &= test_alltoallmem_with_size(1); /* Single byte elements */
  shmem_barrier_all(); /* Ensure all PEs complete before next test */
  result &= test_alltoallmem_with_size(4); /* 4-byte elements */
  shmem_barrier_all(); /* Ensure all PEs complete before next test */
  result &= test_alltoallmem_with_size(8); /* 8-byte elements */
  shmem_barrier_all(); /* Ensure all PEs complete before next test */
  result &= test_alltoallmem_with_size(16); /* 16-byte elements */
  shmem_barrier_all(); /* Ensure all PEs complete before next test */
  result &= test_alltoallmem_with_size(32); /* 32-byte elements */
  shmem_barrier_all(); /* Ensure all PEs complete before next test */
  result &= test_alltoallmem_with_size(37); /* Non-power-of-2 size */

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C shmem_alltoallmem", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
