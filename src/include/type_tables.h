/**
 * @file type_tables.h
 * @brief Type tables for OpenSHMEM
 *
 * For license: see LICENSE file at top-level
 */

#ifndef TYPE_TABLES_H
#define TYPE_TABLES_H 1

#include <sys/types.h>
#include <stdint.h>
#include <complex.h>

/*
 * Standard RMA types (Table 5)
 *
 * Used by:
 * - shmem_put()
 * - shmem_p()
 * - shmem_iput()
 * - shmem_get()
 * - shmem_g()
 * - shmem_iget()
 * - shmem_put_nbi()
 * - shmem_get_nbi()
 * - shmem_put_signal()
 * - shmem_put_signal_nbi()
 *
 * - shmem_alltoall()
 * - shmem_alltoalls()
 * - shmem_broadcast()
 * - shmem_collect()
 * - shmem_fcollect()
 */
#define SHMEM_STANDARD_RMA_TYPE_TABLE(X)                                       \
  X(float, float)                                                              \
  X(double, double)                                                            \
  X(long double, longdouble)                                                   \
  X(char, char)                                                                \
  X(signed char, schar)                                                        \
  X(short, short)                                                              \
  X(int, int)                                                                  \
  X(long, long)                                                                \
  X(long long, longlong)                                                       \
  X(unsigned char, uchar)                                                      \
  X(unsigned short, ushort)                                                    \
  X(unsigned int, uint)                                                        \
  X(unsigned long, ulong)                                                      \
  X(unsigned long long, ulonglong)                                             \
  X(int8_t, int8)                                                              \
  X(int16_t, int16)                                                            \
  X(int32_t, int32)                                                            \
  X(int64_t, int64)                                                            \
  X(uint8_t, uint8)                                                            \
  X(uint16_t, uint16)                                                          \
  X(uint32_t, uint32)                                                          \
  X(uint64_t, uint64)                                                          \
  X(size_t, size)                                                              \
  X(ptrdiff_t, ptrdiff)

// clang-format off
 /*
  * Standard AMO types (Table 6)
  *
  * Used by:
  * - shmem_atomic_compare_swap()
  * - shmem_atomic_fetch_inc()
  * - shmem_atomic_inc()
  * - shmem_atomic_fetch_add()
  * - shmem_atomic_add()
  * - shmem_atomic_compare_swap_nbi()
  * - shmem_atomic_fetch_inc_nbi()
  * - shmem_atomic_fetch_add_nbi()
  *
  * - shmem_wait_until() (FIXME: also need `short` and `ushort`, which are deprecated)
  * - shmem_wait_until_all()
  * - shmem_wait_until_any()
  * - shmem_wait_until_some()
  * - shmem_wait_until_all_vector()
  * - shmem_wait_until_any_vector()
  * - shmem_wait_until_some_vector()
  * - shmem_test() (FIXME: also need `short` and `ushort`, which are deprecated)
  * - shmmem_test_all()
  * - shmem_test_any()
  * - shmem_test_some()
  * - shmem_test_all_vector()
  * - shmem_test_any_vector()
  * - shmem_test_some_vector()
  */
// clang-format on
#define SHMEM_STANDARD_AMO_TYPE_TABLE(X)                                       \
  X(int, int)                                                                  \
  X(long, long)                                                                \
  X(long long, longlong)                                                       \
  X(unsigned int, uint)                                                        \
  X(unsigned long, ulong)                                                      \
  X(unsigned long long, ulonglong)                                             \
  X(int32_t, int32)                                                            \
  X(int64_t, int64)                                                            \
  X(uint32_t, uint32)                                                          \
  X(uint64_t, uint64)                                                          \
  X(size_t, size)                                                              \
  X(ptrdiff_t, ptrdiff)

/*
 * Extended AMO types (Table 7)
 *
 * Used by:
 * - shmem_atomic_fetch()
 * - shmem_atomic_set()
 * - shmem_atomic_swap()
 * - shmem_atomic_fetch_nbi()
 * - shmem_atomic_swap_nbi()
 */
#define SHMEM_EXTENDED_AMO_TYPE_TABLE(X)                                       \
  X(float, float)                                                              \
  X(double, double)                                                            \
  X(int, int)                                                                  \
  X(long, long)                                                                \
  X(long long, longlong)                                                       \
  X(unsigned int, uint)                                                        \
  X(unsigned long, ulong)                                                      \
  X(unsigned long long, ulonglong)                                             \
  X(int32_t, int32)                                                            \
  X(int64_t, int64)                                                            \
  X(uint32_t, uint32)                                                          \
  X(uint64_t, uint64)                                                          \
  X(size_t, size)                                                              \
  X(ptrdiff_t, ptrdiff)

/*
 * Bitwise AMO types (Table 8)
 *
 * Used by:
 * - shmem_atomic_fetch_and()
 * - shmem_atomic_and()
 * - shmem_atomic_fetch_or()
 * - shmem_atomic_or()
 * - shmem_atomic_fetch_xor()
 * - shmem_atomic_xor()
 * - shmem_atomic_fetch_and_nbi()
 * - shmem_atomic_fetch_or_nbi()
 * - shmem_atomic_fetch_xor_nbi()
 */
#define SHMEM_BITWISE_AMO_TYPE_TABLE(X)                                        \
  X(unsigned int, uint)                                                        \
  X(unsigned long, ulong)                                                      \
  X(unsigned long long, ulonglong)                                             \
  X(int32_t, int32)                                                            \
  X(int64_t, int64)                                                            \
  X(uint32_t, uint32)                                                          \
  X(uint64_t, uint64)

/*
 * Bitwise to_all types (Table 11)
 *
 * User by:
 * - shmem_and_to_all()
 * - shmem_or_to_all()
 * - shmem_xor_to_all()
 */
#define SHMEM_TO_ALL_BITWISE_TYPE_TABLE(X)                                     \
  X(short, short)                                                              \
  X(int, int)                                                                  \
  X(long, long)                                                                \
  X(long long, longlong)

/*
 * Minmax to_all types (Table 11)
 *
 * User by:
 * - shmem_min_to_all()
 * - shmem_max_to_all()
 */
#define SHMEM_TO_ALL_MINMAX_TYPE_TABLE(X)                                      \
  X(short, short)                                                              \
  X(int, int)                                                                  \
  X(long, long)                                                                \
  X(long long, longlong)                                                       \
  X(float, float)                                                              \
  X(double, double)                                                            \
  X(long double, longdouble)

/*
 * Arithmetic to_all types (Table 11)
 *
 * User by:
 * - shmem_sum_to_all()
 * - shmem_prod_to_all()
 */
#define SHMEM_TO_ALL_ARITH_TYPE_TABLE(X)                                       \
  X(short, short)                                                              \
  X(int, int)                                                                  \
  X(long, long)                                                                \
  X(long long, longlong)                                                       \
  X(float, float)                                                              \
  X(double, double)                                                            \
  X(long double, longdouble)                                                   \
  X(double _Complex, complexd)                                                 \
  X(float _Complex, complexf)

/*
 * Team-based reduce bitwise operations (Table 10)
 *
 * User by:
 * - shmem_and_reduce()
 * - shmem_or_reduce()
 * - shmem_xor_reduce()
 */
#define SHMEM_REDUCE_BITWISE_TYPE_TABLE(X)                                     \
  X(unsigned char, uchar)                                                      \
  X(unsigned short, ushort)                                                    \
  X(unsigned int, uint)                                                        \
  X(unsigned long, ulong)                                                      \
  X(unsigned long long, ulonglong)                                             \
  X(int8_t, int8)                                                              \
  X(int16_t, int16)                                                            \
  X(int32_t, int32)                                                            \
  X(int64_t, int64)                                                            \
  X(uint8_t, uint8)                                                            \
  X(uint16_t, uint16)                                                          \
  X(uint32_t, uint32)                                                          \
  X(uint64_t, uint64)                                                          \
  X(size_t, size)

/*
 * Team-based reduce minmax operations (Table 10)
 *
 * User by:
 * - shmem_min_reduce()
 * - shmem_max_reduce()
 */
#define SHMEM_REDUCE_MINMAX_TYPE_TABLE(X)                                      \
  X(char, char)                                                                \
  X(signed char, schar)                                                        \
  X(short, short)                                                              \
  X(int, int)                                                                  \
  X(long, long)                                                                \
  X(long long, longlong)                                                       \
  X(ptrdiff_t, ptrdiff)                                                        \
  X(unsigned char, uchar)                                                      \
  X(unsigned short, ushort)                                                    \
  X(unsigned int, uint)                                                        \
  X(unsigned long, ulong)                                                      \
  X(unsigned long long, ulonglong)                                             \
  X(int8_t, int8)                                                              \
  X(int16_t, int16)                                                            \
  X(int32_t, int32)                                                            \
  X(int64_t, int64)                                                            \
  X(uint8_t, uint8)                                                            \
  X(uint16_t, uint16)                                                          \
  X(uint32_t, uint32)                                                          \
  X(uint64_t, uint64)                                                          \
  X(size_t, size)                                                              \
  X(float, float)                                                              \
  X(double, double)                                                            \
  X(long double, longdouble)

/*
 * Team-based reduce arithmetic operations (Table 10)
 *
 * User by:
 * - shmem_sum_reduce()
 * - shmem_prod_reduce()
 */
#define SHMEM_REDUCE_ARITH_TYPE_TABLE(X)                                       \
  X(char, char)                                                                \
  X(signed char, schar)                                                        \
  X(short, short)                                                              \
  X(int, int)                                                                  \
  X(long, long)                                                                \
  X(long long, longlong)                                                       \
  X(ptrdiff_t, ptrdiff)                                                        \
  X(unsigned char, uchar)                                                      \
  X(unsigned short, ushort)                                                    \
  X(unsigned int, uint)                                                        \
  X(unsigned long, ulong)                                                      \
  X(unsigned long long, ulonglong)                                             \
  X(int8_t, int8)                                                              \
  X(int16_t, int16)                                                            \
  X(int32_t, int32)                                                            \
  X(int64_t, int64)                                                            \
  X(uint8_t, uint8)                                                            \
  X(uint16_t, uint16)                                                          \
  X(uint32_t, uint32)                                                          \
  X(uint64_t, uint64)                                                          \
  X(size_t, size)                                                              \
  X(float, float)                                                              \
  X(double, double)                                                            \
  X(long double, longdouble)                                                   \
  X(double _Complex, complexd)                                                 \
  X(float _Complex, complexf)

/*
 * Deprecated point-to-point synchronization types (Table 12)
 *
 * Used by:
 * - shmem_wait()
 */
#define SHMEM_DEPR_PT2PT_SYNC_TYPE_TABLE(X)                                    \
  X(short, short)                                                              \
  X(int, int)                                                                  \
  X(long, long)                                                                \
  X(long long, longlong)                                                       \
  X(unsigned short, ushort)                                                    \
  X(unsigned int, uint)                                                        \
  X(unsigned long, ulong)                                                      \
  X(unsigned long long, ulonglong)                                             \
  X(int32_t, int32)                                                            \
  X(int64_t, int64)                                                            \
  X(uint32_t, uint32)                                                          \
  X(uint64_t, uint64)                                                          \
  X(size_t, size)                                                              \
  X(ptrdiff_t, ptrdiff)

#endif /* TYPE_TABLES_H */