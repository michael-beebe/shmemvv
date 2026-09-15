/**
 * @file c11_shmem_signal_set.c
 * @brief Unit test for the shmem_signal_set() routine.
 */
#include <shmem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "log.h"
#include "shmemvv.h"

#define TIMEOUT_MS 300

bool test_c11_shmem_signal_set(){
  log_routine("shmem_signal_set()");
  uint64_t *signal = (uint64_t *)shmem_malloc(sizeof(uint64_t));
  int mype = shmem_my_pe();
  int npes = shmem_n_pes();

  if (signal == NULL) {
    log_fail("Failed to allocate symmetric memory");
    return false;
  } else if (npes < 2) {
    log_fail("Test requires at least 2 PEs, but only %d PE(s) available",
             npes);
    return false;
  }

  int target_pe = 1; /* Simple: PE 0 sends to PE 1 */
  log_info("signal @ %p", signal);
  /* Initialize values */
  *signal = 0;
  shmem_barrier_all();
  if (mype == 0) {
    log_info("calling shmem_signal_set() signal=%p, target_pe=%d)",
             signal, target_pe);
    shmem_signal_set(signal, 1, target_pe);
  }
  
  shmem_barrier_all();
  if (mype == 1) {
    /* Wait with timeout for signal to be set */
    long start = current_time_ms();
    while(!shmem_test(signal, SHMEM_CMP_EQ, 1)){
      if (current_time_ms() - start > TIMEOUT_MS){
        log_fail("validation failed: signal = %d (expected 1)",(int)*signal);
        return false;
      }
    }
    log_info("result is valid");
  }
  shmem_barrier_all(); 
  shmem_free(signal);
  return true;
}

bool test_c11_ctx_shmem_signal_set(){
  log_routine("shmem_signal_set()");
  uint64_t *signal = (uint64_t *)shmem_malloc(sizeof(uint64_t));
  int mype = shmem_my_pe();
  int npes = shmem_n_pes();
  if (!signal) {
    log_fail("Failed to allocate symmetric memory");
    return false;
  } else if (npes < 2) {
    log_fail("Test requires at least 2 PEs, but only %d PE(s) available",
             npes);
    return false;
  }

  shmem_ctx_t ctx;
  int ctx_create_status = shmem_ctx_create(0, &ctx);
  if (ctx_create_status != 0) {
    log_fail("Failed to create context");
    return false;
  }

  int target_pe = 1; /* Simple: PE 0 sends to PE 1 */
  log_info("signal @ %p", signal);
  /* Initialize values */
  *signal = 0;
  shmem_barrier_all();
  if (mype == 0) {
    log_info("calling shmem_signal_set() signal=%p, target_pe=%d)",
             signal, target_pe);
    shmem_signal_set(ctx, signal, 1, target_pe);
  }

  shmem_barrier_all();
  if (mype == 1) {
    /* Wait with timeout for signal to be set */
    long start = current_time_ms();
    while(!shmem_test(signal, SHMEM_CMP_EQ, 1)){
      if (start - current_time_ms() > TIMEOUT_MS){
        log_fail("validation failed: signal = %d (expected 1)",(int)*signal);
        return false;
      }
    }
    log_info("result is valid");
  }
  shmem_barrier_all(); 
  shmem_free(signal);
  return true;
}

int main(int argc, char *argv[]) {
  shmem_init();
  log_init(__FILE__);

  int npes = shmem_n_pes();
  int mype = shmem_my_pe();

  if (!(npes >= 2)) {
    if (mype == 0) {
      display_not_enough_pes("SIGNALING");
    }
    shmem_finalize();
    return EXIT_SUCCESS;
  }
  static bool result;
  static bool result_ctx;

  /* Test standard C11 shmem_signal_set */
  result = test_c11_shmem_signal_set();
  reduce_test_result("C11 shmem_signal_set", &result, false);

  /* Test context-specific variant in C11 */
  result_ctx = test_c11_ctx_shmem_signal_set(); 

  reduce_test_result("C11 shmem_signal_set with ctx", &result_ctx, false);

  bool passed = result & result_ctx;
  log_close(!passed);
  shmem_finalize();
  return passed ? EXIT_SUCCESS : EXIT_FAILURE;
}
