/**
 * @file c_shmem_alltoallsmem.c
 *
 * @brief Unit test for shmem_alltoallsmem().
 */

#include <shmem.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "log.h"
#include "shmemvv.h"

/**
 * Test function for shmem_alltoallmem with configurable element number and
 * strides
 */
static bool test_alltoallsmem_with_params(size_t n_elems,
                                          ptrdiff_t dst_stride,
                                          ptrdiff_t src_stride) {
  char routine_name[64];
  snprintf(routine_name, sizeof(routine_name),
           "shmem_alltoallsmem(elem_size=%zu, dst=%td, src=%td)", n_elems,
           dst_stride, src_stride);
  log_routine(routine_name);

  int npes = shmem_n_pes();
  int mype = shmem_my_pe();

  /* Calculate buffer sizes according to OpenSHMEM spec */
  /* For alltoallsmem, elem_size=1, so buffer size = stride * 1 * npes * n_elems
   */
  size_t total_src_bytes = src_stride * 1 * npes * n_elems;
  size_t total_dst_bytes = dst_stride * 1 * npes * n_elems;
  size_t total_elems = n_elems * npes;

  void *src = shmem_malloc(total_src_bytes);
  void *dest = shmem_malloc(total_dst_bytes);

  if (src == NULL || dest == NULL) {
    log_fail("Failed to allocate memory for src or dest");
    if (src)
      shmem_free(src);
    if (dest)
      shmem_free(dest);
    return false;
  }

  log_info("shmem_malloc'd %zu bytes @ &src = %p, %zu bytes @ &dest = %p",
           total_src_bytes, src, total_dst_bytes, dest);

  /* Initialize everything to a known pattern */
  memset(src, 0, total_src_bytes);
  memset(dest, 0xFF, total_dst_bytes);

  /*
   * Initialize source array with test pattern.
   * Each PE puts its data at specific stride positions for each destination PE.
   */
  for (int elem = 0; elem < total_elems; elem++) {
    /* Source offset: elem * src_stride */
    size_t offset = elem * src_stride;
    unsigned char *src_ptr = (unsigned char *)src + offset;

    /* Create a distinct value for each destination PE */
    unsigned char value = (unsigned char)((mype * 10 + elem) % 256);

    /* Fill each byte block with the value */
    memset(src_ptr, value, 1);

    log_info("Source: PE %d setting elem for element %d at offset %zu with value %d",
             mype, elem, offset, value);
  }

  /* Execute alltoallsmem */
  log_info(
      "executing shmem_alltoallsmem: dest = %p, src = %p, dst_stride = %td, "
      "src_stride = %td, n_elems = %zu",
      dest, src, dst_stride, src_stride, n_elems);

  shmem_barrier_all(); /* Ensure all PEs are ready */
  shmem_alltoallsmem(SHMEM_TEAM_WORLD, dest, src, dst_stride, src_stride,
                     n_elems);
  shmem_barrier_all(); /* Ensure all PEs complete the operation */

  /* Validate results */
  log_info("validating result...");
  bool success = true;

  for (int elem = 0; elem < total_elems; elem++) {
    /* Destination offset: (pe * 1 + 0) * dst_stride * elem_size = pe *
     * dst_stride * elem_size */
    size_t offset = elem * dst_stride;
    unsigned char *dest_ptr = (unsigned char *)dest + offset;

    /* Expected value from PE pe, result of all to all pattern */
    unsigned char expected = (unsigned char) ((elem / n_elems) * 10 + (n_elems * mype) + (elem % n_elems));
    
    /* check that the value of each byte is correct */
    if (*dest_ptr != expected) {
      log_info("PE %d: Data from elem %d at offset %zu failed. Expected "
                "%d, got %d",
                mype, elem, offset, expected, *dest_ptr);
      success = false;  
      }
    if (!success){
      break;
    }
  }

  if (success)
    log_info("shmem_alltoallsmems with n_elems %zu, dst stride %td, src "
             "stride %td produced expected result.",
             n_elems, dst_stride, src_stride);
  else
    log_fail(
        "at least one value was unexpected in result of shmem_alltoallsmem "
        "with n_elems %zu, dst stride %td, src stride %td",
        n_elems, dst_stride, src_stride);

  /* Print sample results for first few PEs */
  if (mype < 2) {
    log_info("PE %d: Sample results:", mype);
    for (int elem = 0; elem < total_elems && elem < 3; elem++) {
      size_t offset = elem * dst_stride;
      unsigned char *dest_ptr = (unsigned char *)dest + offset;
      log_info("  dest[%zu] = %d", offset, (int)dest_ptr[0]);
    }
  }

  shmem_free(src);
  shmem_free(dest);

  return success;
}

int main(int argc, char *argv[]) {
  shmem_init();
  log_init(__FILE__);

  static bool result = true;
  /* Test shmem_alltoallsmem with various element sizes and strides */

  // clang-format off
  
  /* Basic tests with contiguous arrays (stride=1) */
  result &= test_alltoallsmem_with_params(8, 1, 1);  /* 8-byte elements */
  shmem_barrier_all();
  result &= test_alltoallsmem_with_params(4, 1, 1);  /* 4-byte elements */
  shmem_barrier_all();
  result &= test_alltoallsmem_with_params(1, 1, 1);  /* 1-byte elements */
  shmem_barrier_all();
  
  /* Test with non-contiguous data (stride>1) */
  result &= test_alltoallsmem_with_params(8, 2, 2);  /* 8-byte elements, stride=2 */
  shmem_barrier_all();
  result &= test_alltoallsmem_with_params(4, 3, 3);  /* 4-byte elements, stride=3 */
  shmem_barrier_all();
  
  /* Testing with different strides for source and destination */
  result &= test_alltoallsmem_with_params(8, 2, 3);  /* 8-byte elements, different strides */
  shmem_barrier_all();
  result &= test_alltoallsmem_with_params(4, 3, 2);  /* 4-byte elements, different strides */
  shmem_barrier_all();
  
  /* Test with various sizes */
  result &= test_alltoallsmem_with_params(12, 2, 2); /* 12-byte elements */
  shmem_barrier_all();
  result &= test_alltoallsmem_with_params(16, 2, 2); /* 16-byte elements */
  shmem_barrier_all();
  result &= test_alltoallsmem_with_params(32, 2, 2); /* 32-byte elements */

  // clang-format on

  shmem_barrier_all();

  reduce_test_result("C shmem_alltoallsmem", &result, false);

  log_close(!result);
  shmem_finalize();
  return result ? EXIT_SUCCESS : EXIT_FAILURE;
}
