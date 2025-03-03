/**
 * @file c_shmem_collectmem.c
 * @brief Unit test for shmem_collectmem().
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "log.h"
#include "shmemvv.h"

/**
 * Test function for shmem_collectmem with configurable element size
 *
 * @param nelems Number of bytes each PE will contribute to the collect
 * @return true if test passed, false otherwise
 */
static bool test_collectmem(size_t nelems) {
  char routine_name[64];
  snprintf(routine_name, sizeof(routine_name), "shmem_collectmem (nelems=%zu)",
           nelems);
  log_routine(routine_name);

  int npes = shmem_n_pes();
  int mype = shmem_my_pe();

  /* Each PE contributes exactly 'nelems' bytes */
  size_t total_nelems = npes * nelems;

  /* Allocate source and destination buffers */
  void *src = shmem_malloc(nelems);
  void *dest = shmem_malloc(total_nelems);

  if (src == NULL || dest == NULL) {
    log_fail("Failed to allocate memory for src or dest");
    if (src)
      shmem_free(src);
    if (dest)
      shmem_free(dest);
    return false;
  }

  log_info("shmem_malloc'd %zu bytes @ &src = %p, %zu bytes @ &dest = %p",
           nelems, src, total_nelems, dest);

  /* Initialize source buffer with unique values for each PE */
  unsigned char *src_ptr = (unsigned char *)src;

  /* Initialize with a simple pattern based on PE number */
  for (size_t i = 0; i < nelems; i++) {
    src_ptr[i] = (unsigned char)(100 + mype);

    /* Only log a few elements to avoid log file bloat */
    if (i < 5 || i > nelems - 5) {
      log_info("set src[%zu] = %d", i, src_ptr[i]);
    } else if (i == 5) {
      log_info("... (skipping detailed logging of middle elements)");
    }
  }

  /* Initialize destination with a different pattern */
  memset(dest, 0xFF, total_nelems);

  shmem_barrier_all(); /* Ensure all PEs are ready */

  /* Execute the collect operation */
  log_info("executing shmem_collectmem: dest = %p, src = %p, nelems = %zu",
           dest, src, nelems);

  int collect_rc = shmem_collectmem(SHMEM_TEAM_WORLD, dest, src, nelems);

  if (collect_rc != 0) {
    log_fail("shmem_collectmem returned non-zero: %d", collect_rc);
    shmem_free(src);
    shmem_free(dest);
    return false;
  }

  shmem_barrier_all(); /* Ensure all PEs complete the operation */

  /* Validate result */
  log_info("validating result...");
  bool success = true;
  unsigned char *dest_ptr = (unsigned char *)dest;

  /* Check that data from each PE is correct */
  for (int pe = 0; pe < npes; pe++) {
    size_t pe_offset = pe * nelems;
    unsigned char expected = (unsigned char)(100 + pe);

    for (size_t i = 0; i < nelems; i++) {
      if (dest_ptr[pe_offset + i] != expected) {
        log_fail("Data from PE %d at offset %zu failed. Expected %d, got %d",
                 pe, pe_offset + i, expected, dest_ptr[pe_offset + i]);
        success = false;
        break;
      }
    }

    if (!success)
      break;
  }

  if (success)
    log_info("shmem_collectmem with nelems=%zu produced expected result.",
             nelems);
  else
    log_fail("At least one value was unexpected in result of shmem_collectmem "
             "with nelems=%zu",
             nelems);

  shmem_free(src);
  shmem_free(dest);

  return success;
}

int main(int argc, char *argv[]) {
  shmem_init();
  log_init(__FILE__);

  bool result = true;
  int rc = EXIT_SUCCESS;

  /* Test shmem_collectmem with various data sizes */

  // clang-format off
  
  /* Start with small sizes */
  result &= test_collectmem(1);      /* 1 byte */
  
  if (result) {
    result &= test_collectmem(4);      /* 4 bytes */
    result &= test_collectmem(8);      /* 8 bytes */
    result &= test_collectmem(16);     /* 16 bytes */
    result &= test_collectmem(64);     /* 64 bytes */
    
    /* Test with larger sizes */
    result &= test_collectmem(1024);   /* 1 KB */
  }

  // clang-format on

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C shmem_collectmem", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();

  return rc;
}
