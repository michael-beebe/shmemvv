/**
 * @file c_shmem_broadcastmem.c
 * @brief Unit test for shmem_broadcastmem().
 */

#include "log.h"
#include "shmemvv.h"

/**
 * Test function for shmem_broadcastmem with configurable element size
 */
static bool test_broadcastmem_with_size(size_t elem_size) {
  char routine_name[64];
  snprintf(routine_name, sizeof(routine_name), "shmem_broadcastmem (%zu bytes)",
           elem_size);
  log_routine(routine_name);

  int npes = shmem_n_pes();
  int mype = shmem_my_pe();

  /* Calculate total bytes needed for source and destination buffers */
  size_t total_bytes = elem_size * 4; /* Testing with 4 elements */

  void *src = shmem_malloc(total_bytes);
  void *dest = shmem_malloc(total_bytes);

  if (src == NULL || dest == NULL) {
    log_fail("Failed to allocate memory for src or dest");
    if (src)
      shmem_free(src);
    if (dest)
      shmem_free(dest);
    return false;
  }

  log_info("shmem_malloc'd %zu bytes @ &src = %p, %zu bytes @ &dest = %p",
           total_bytes, src, total_bytes, dest);

  /* Initialize buffer with known pattern on all PEs */
  memset(src, 0, total_bytes);
  memset(dest, 0xFF, total_bytes);

  /* Only root PE sets source data */
  if (mype == 0) {
    unsigned char *src_ptr = (unsigned char *)src;
    for (size_t i = 0; i < total_bytes; i++) {
      src_ptr[i] = (unsigned char)((i % 256) + 1); /* +1 to avoid zeros */
    }
    log_info("Root PE initialized source buffer with test pattern");
  }

  shmem_barrier_all(); /* Ensure all PEs are ready */

  /* Execute broadcastmem */
  log_info("executing shmem_broadcastmem: dest = %p, src = %p, nelems = %zu, "
           "root = 0",
           dest, src, total_bytes);

  int rc = shmem_broadcastmem(SHMEM_TEAM_WORLD, dest, src, total_bytes, 0);

  if (rc != 0) {
    log_fail("shmem_broadcastmem returned non-zero: %d", rc);
    shmem_free(src);
    shmem_free(dest);
    return false;
  }

  shmem_barrier_all(); /* Ensure all PEs complete the operation */

  /* Validate results */
  log_info("validating result...");
  bool success = true;

  /* Create expected pattern locally */
  unsigned char *expected = (unsigned char *)malloc(total_bytes);
  if (!expected) {
    log_fail("Failed to allocate memory for expected values");
    shmem_free(src);
    shmem_free(dest);
    return false;
  }

  /* Fill with the same pattern that PE 0 would have used */
  for (size_t i = 0; i < total_bytes; i++) {
    expected[i] = (unsigned char)((i % 256) + 1);
  }

  /* Compare destination with expected values */
  unsigned char *dest_ptr = (unsigned char *)dest;
  for (size_t i = 0; i < total_bytes; i++) {
    if (dest_ptr[i] != expected[i]) {
      log_fail("Byte %zu of dest failed validation. Expected %d, got %d", i,
               expected[i], dest_ptr[i]);
      success = false;
      break;
    }
  }

  if (success)
    log_info(
        "shmem_broadcastmem with element size %zu produced expected result.",
        elem_size);
  else
    log_fail("at least one value was unexpected in result of "
             "shmem_broadcastmem with element size %zu",
             elem_size);

  free(expected);
  shmem_free(src);
  shmem_free(dest);

  return success;
}

int main(int argc, char *argv[]) {
  shmem_init();
  log_init(__FILE__);

  bool result = true;
  int rc = EXIT_SUCCESS;

  /* Test shmem_broadcastmem with various element sizes */

  // clang-format off
  
  /* Start with smaller sizes to help debug */
  result &= test_broadcastmem_with_size(1);    /* 1-byte elements */
  result &= test_broadcastmem_with_size(4);    /* 4-byte elements */
  result &= test_broadcastmem_with_size(8);    /* 8-byte elements */
  
  /* If those pass, try larger sizes */
  if (result) {
    result &= test_broadcastmem_with_size(16);   /* 16-byte elements */
    result &= test_broadcastmem_with_size(32);   /* 32-byte elements */
    result &= test_broadcastmem_with_size(64);   /* 64-byte elements */
    result &= test_broadcastmem_with_size(128);  /* 128-byte elements */
    
    /* Test with odd sizes to check alignment handling */
    result &= test_broadcastmem_with_size(3);    /* 3-byte elements */
    result &= test_broadcastmem_with_size(7);    /* 7-byte elements */
    result &= test_broadcastmem_with_size(13);   /* 13-byte elements */
  }

  // clang-format on

  shmem_barrier_all();

  if (shmem_my_pe() == 0) {
    display_test_result("C shmem_broadcastmem", result, false);
  }

  if (!result) {
    rc = EXIT_FAILURE;
  }

  log_close(rc);
  shmem_finalize();

  return rc;
}
