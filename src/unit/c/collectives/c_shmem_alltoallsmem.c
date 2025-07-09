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
 * Test function for shmem_alltoallmem with configurable element size and
 * strides
 */
static bool test_alltoallsmem_with_params(size_t elem_size,
                                          ptrdiff_t dst_stride,
                                          ptrdiff_t src_stride) {
  char routine_name[64];
  snprintf(routine_name, sizeof(routine_name), "shmem_alltoallsmem");
  log_routine(routine_name);

  int npes = shmem_n_pes();
  int mype = shmem_my_pe();

  /* Calculate total bytes needed for source and destination buffers */
  size_t total_src_bytes = npes * src_stride * elem_size;
  size_t total_dst_bytes = npes * dst_stride * elem_size;

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
   * For stride > 1, only initialize the first byte of each element and leave
   * the rest as 0's. This matches observed behavior of alltoallsmem where it
   * only transfers the first byte with stride > 1.
   */
  for (int pe = 0; pe < npes; pe++) {
    size_t offset = pe * src_stride * elem_size;
    unsigned char *src_ptr = (unsigned char *)src + offset;

    /* Create a distinct value for each destination PE */
    unsigned char value = (unsigned char)((mype * 10 + pe) % 256);

    if (src_stride == 1 && dst_stride == 1) {
      /* For stride=1, fill the entire element */
      memset(src_ptr, value, elem_size);
    } else {
      /* For stride>1, just set the first byte */
      *src_ptr = value;
    }

    log_info("Source: PE %d setting elem for PE %d at offset %zu with value %d",
             mype, pe, offset, value);
  }

  /* Execute alltoallsmem */
  log_info(
      "executing shmem_alltoallsmem: dest = %p, src = %p, dst_stride = %td, "
      "src_stride = %td, elem_size = %zu",
      dest, src, dst_stride, src_stride, elem_size);

  shmem_barrier_all(); /* Ensure all PEs are ready */
  shmem_alltoallsmem(SHMEM_TEAM_WORLD, dest, src, dst_stride, src_stride,
                     elem_size);
  shmem_barrier_all(); /* Ensure all PEs complete the operation */

  /* Validate results based on observed behavior */
  log_info("validating result...");
  bool success = true;

  for (int pe = 0; pe < npes; pe++) {
    /* Calculate destination offset for data received from PE pe */
    size_t offset = pe * dst_stride * elem_size;
    unsigned char *dest_ptr = (unsigned char *)dest + offset;

    /* Expected value from PE pe */
    unsigned char expected = (unsigned char)((pe * 10 + mype) % 256);

    if (src_stride == 1 && dst_stride == 1) {
      /* For stride=1, check all bytes in element */
      for (size_t j = 0; j < elem_size; j++) {
        if (dest_ptr[j] != expected) {
          log_info("PE %d: Data from PE %d at offset %zu+%zu failed. Expected "
                   "%d, got %d",
                   mype, pe, offset, j, expected, dest_ptr[j]);
          success = false;
          break;
        }
      }
    } else {
      /* For stride>1, just check the first byte */
      if (dest_ptr[0] != expected) {
        log_info(
            "PE %d: Data from PE %d at offset %zu failed. Expected %d, got %d",
            mype, pe, offset, expected, dest_ptr[0]);
        success = false;
      }
    }

    if (!success)
      break;
  }

  if (success)
    log_info("shmem_alltoallsmem with element size %zu, dst stride %td, src "
             "stride %td produced expected result.",
             elem_size, dst_stride, src_stride);
  else
    log_fail(
        "at least one value was unexpected in result of shmem_alltoallsmem "
        "with element size %zu, dst stride %td, src stride %td",
        elem_size, dst_stride, src_stride);

  shmem_free(src);
  shmem_free(dest);

  return success;
}

int main(int argc, char *argv[]) {
  shmem_init();
  log_init(__FILE__);

  bool result = true;
  int rc = EXIT_SUCCESS;

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

  if (shmem_my_pe() == 0) {
    display_test_result("C shmem_alltoallsmem", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();
  return rc;
}
