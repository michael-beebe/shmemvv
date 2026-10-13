/**
 * @file c_shmem_sum_exscan.c
 * @brief Sum scan over world, strided and singleton teams, typed and C11.
 */
#include <shmem.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "log.h"
#include "shmemvv.h"
#include "type_tables.h"

#ifdef API_USE_GENERICS
#define SCAN(TYPE, NAME, TEAM, DEST, SOURCE, N)                                \
  shmem_sum_exscan(TEAM, DEST, SOURCE, N)
#else
#define SCAN(TYPE, NAME, TEAM, DEST, SOURCE, N)                                \
  shmem_##NAME##_sum_exscan(TEAM, DEST, SOURCE, N)
#endif
#define SCAN_EXCLUSIVE 1
static bool values_result;
#include "sum_scan_test.h"

int main(int argc, char **argv) {
  shmem_init();
  log_init(__FILE__);
  bool subteam = argc > 1 && strcmp(argv[1], "--subteam") == 0;
  bool singleton = argc > 1 && strcmp(argv[1], "--singleton") == 0;
  shmem_team_t team = SHMEM_TEAM_WORLD;
  if (subteam || singleton) {
    int size = singleton ? 1 : (shmem_n_pes() + 1) / 2;
    int stride = singleton ? 1 : 2;
    if (shmem_team_split_strided(SHMEM_TEAM_WORLD, 0, stride, size, NULL, 0,
                                 &team) != 0) {
      log_fail("Team split failed");
      shmem_global_exit(EXIT_FAILURE);
    }
  }
  static bool result;
  result = values_result = true;
  if (team != SHMEM_TEAM_INVALID) {
#define TEST_TYPE(TYPE, NAME) result &= TEST_C_SHMEM_SUM_SCAN(TYPE, NAME, team);
    SHMEM_REDUCE_ARITH_TYPE_TABLE(TEST_TYPE)
#undef TEST_TYPE
  }
  shmem_barrier_all();
  reduce_test_result("shmem_sum_exscan values", &values_result, false);
  reduce_test_result("shmem_sum_exscan empty return contract", &result, false);
  shmem_barrier_all();
  int rc = result && values_result ? EXIT_SUCCESS : EXIT_FAILURE;
  if ((subteam || singleton) && team != SHMEM_TEAM_INVALID)
    shmem_team_destroy(team);
  log_close(rc);
  shmem_finalize();
  return rc;
}
