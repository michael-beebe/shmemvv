/**
 * @file shmemvv_teams.h
 * @brief Helper routines and utilities for OpenSHMEM team unit tests
 */

#ifndef SHMEMVV_TEAMS_H
#define SHMEMVV_TEAMS_H

#include <shmem.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief The world PE that should be team PE @p i of the given triplet.
 */
static inline int triplet_member(int start, int stride, int i) {
  return start + stride * i;
}

/**
 * @brief This PE's expected number in the team named by the triplet, or -1 if
 * it is not a member.
 */
static inline int expected_team_pe(int start, int stride, int size) {
  int mype = shmem_my_pe();
  for (int i = 0; i < size; ++i) {
    if (triplet_member(start, stride, i) == mype) {
      return i;
    }
  }
  return -1;
}

/**
 * @brief Whether this PE is named by the triplet.
 */
static inline bool triplet_names_me(int start, int stride, int size) {
  return expected_team_pe(start, stride, size) >= 0;
}

#ifdef __cplusplus
}
#endif

#endif /* SHMEMVV_TEAMS_H */
