// /**
//   @file routines.hpp
//   @brief Contains routine pointers to avoid compiler errors if a routine is not
//          implemented
//  */

// #pragma once

// #include <shmem.h> /* Include OpenSHMEM header for types and constants */

// /************* Define function pointers for OpenSHMEM routines *************/
// extern "C" {

// /* Fake routine for testing */
// typedef void (*shmem_fake_routine_func)(void);

// /* Library Setup, Exit, and Query Routines */
// typedef void (*shmem_init_func)(void);
// typedef void (*shmem_finalize_func)(void);
// typedef int (*shmem_my_pe_func)(void);
// typedef int (*shmem_n_pes_func)(void);
// typedef int (*shmem_pe_accessible_func)(int pe);
// typedef void (*shmem_barrier_all_func)(void);
// typedef void (*shmem_barrier_func)(int PE_start, int logPE_stride, int PE_size, long *pSync);
// typedef void (*shmem_info_get_version_func)(int *major, int *minor);
// typedef void (*shmem_info_get_name_func)(char *name);
// typedef void (*shmem_global_exit_func)(int status);

// /* Thread Support */
// typedef int (*shmem_init_thread_func)(int requested, int *provided);
// typedef int (*shmem_query_thread_func)(int *provided);

// /* Memory Management Routines */
// typedef void* (*shmem_ptr_func)(const void *dest, int pe);
// typedef void* (*shmem_malloc_func)(size_t size);
// typedef void (*shmem_free_func)(void *ptr);
// typedef void* (*shmem_realloc_func)(void *ptr, size_t size);
// typedef void* (*shmem_align_func)(size_t alignment, size_t size);
// typedef void* (*shmem_malloc_with_hints_func)(size_t size, long hints);
// typedef void* (*shmem_calloc_func)(size_t count, size_t size);
// typedef int (*shmem_addr_accessible_func)(const void *addr, int pe);

// /* Team Management Routines */
// typedef int (*shmem_team_my_pe_func)(shmem_team_t team);
// typedef int (*shmem_team_n_pes_func)(shmem_team_t team);
// typedef void (*shmem_team_get_config_func)(shmem_team_t team, shmem_team_config_t *config);
// typedef int (*shmem_team_translate_pe_func)(shmem_team_t src_team, int src_pe, shmem_team_t dest_team);
// typedef shmem_team_t (*shmem_team_split_strided_func)(shmem_team_t parent_team, int start, int stride, int size, const shmem_team_config_t *config, long config_mask, shmem_team_t *new_team);
// typedef shmem_team_t (*shmem_team_split_2d_func)(shmem_team_t parent_team, int xrange, const shmem_team_config_t *xaxis_config, long xaxis_mask, shmem_team_t *xaxis_team, const shmem_team_config_t *yaxis_config, long yaxis_mask, shmem_team_t *yaxis_team);
// typedef void (*shmem_team_destroy_func)(shmem_team_t team);

// /* Communication/Context Management Routines */
// typedef int (*shmem_ctx_create_func)(long options, shmem_ctx_t *ctx);
// typedef int (*shmem_team_create_ctx_func)(shmem_team_t team, long options, shmem_ctx_t *ctx);
// typedef void (*shmem_ctx_destroy_func)(shmem_ctx_t ctx);
// typedef int (*shmem_ctx_get_team_func)(shmem_ctx_t ctx, shmem_team_t *team);

// /* Remote Access Routines */
// typedef void (*shmem_long_put_func)(long *dest, const long *src, size_t nelems, int pe);
// typedef void (*shmem_long_p_func)(long *dest, long value, int pe);
// typedef void (*shmem_long_iput_func)(long *dest, const long *src, ptrdiff_t tst, ptrdiff_t sst, size_t nelems, int pe);
// typedef void (*shmem_long_get_func)(long *dest, const long *src, size_t nelems, int pe);
// typedef long (*shmem_long_g_func)(const long *src, int pe);
// typedef void (*shmem_long_iget_func)(long *dest, const long *src, ptrdiff_t tst, ptrdiff_t sst, size_t nelems, int pe);
// typedef void (*shmem_long_put_nbi_func)(long *dest, const long *src, size_t nelems, int pe);
// typedef void (*shmem_long_get_nbi_func)(long *dest, const long *src, size_t nelems, int pe);

// /* Atomic Memory Operations */
// typedef long (*shmem_long_atomic_fetch_func)(const long *target, int pe);
// typedef void (*shmem_long_atomic_set_func)(long *target, long value, int pe);
// typedef long (*shmem_long_atomic_compare_swap_func)(long *target, long cond, long value, int pe);
// typedef long (*shmem_long_atomic_swap_func)(long *target, long value, int pe);
// typedef long (*shmem_long_atomic_fetch_inc_func)(const long *target, int pe);
// typedef void (*shmem_long_atomic_inc_func)(long *target, int pe);
// typedef long (*shmem_long_atomic_fetch_add_func)(const long *target, long value, int pe);
// typedef void (*shmem_long_atomic_add_func)(const long *target, long value, int pe);
// typedef long (*shmem_long_atomic_fetch_and_func)(const long *target, long value, int pe);
// typedef void (*shmem_long_atomic_and_func)(const long *target, long value, int pe);
// typedef long (*shmem_long_atomic_fetch_or_func)(const long *target, long value, int pe);
// typedef void (*shmem_long_atomic_or_func)(const long *target, long value, int pe);
// typedef long (*shmem_long_atomic_fetch_xor_func)(const long *target, long value, int pe);
// typedef void (*shmem_long_atomic_xor_func)(const long *target, long value, int pe);
// typedef void (*shmem_long_atomic_fetch_nbi_func)(long *dest, const long *target, int pe);
// typedef void (*shmem_long_atomic_compare_swap_nbi_func)(long *dest, long *target, long cond, long value, int pe);
// typedef void (*shmem_long_atomic_swap_nbi_func)(long *dest, long *target, long value, int pe);
// typedef void (*shmem_long_atomic_fetch_inc_nbi_func)(long *dest, const long *target, int pe);
// typedef void (*shmem_long_atomic_fetch_add_nbi_func)(long *dest, const long *target, long value, int pe);
// typedef void (*shmem_long_atomic_fetch_and_nbi_func)(long *dest, const long *target, long value, int pe);
// typedef void (*shmem_long_atomic_fetch_or_nbi_func)(long *dest, const long *target, long value, int pe);
// typedef void (*shmem_long_atomic_fetch_xor_nbi_func)(long *dest, const long *target, long value, int pe);

// /* Signaling Operations */
// typedef void (*shmem_long_put_signal_func)(long *dest, const long *source, size_t nelems, uint64_t *sig_addr, uint64_t signal, int sig_op, int pe);
// typedef void (*shmem_long_put_signal_nbi_func)(long *dest, const long *source, size_t nelems, uint64_t *sig_addr, uint64_t signal, int sig_op, int pe);
// typedef long (*shmem_signal_fetch_func)(const uint64_t *sig_addr);

// /* Collective Routines */
// typedef int (*shmem_sync_func)(int PE_start, int logPE_stride, int PE_size, long *pSync);
// typedef void (*shmem_sync_all_func)(void);
// typedef int (*shmem_long_alltoall_func)(shmem_team_t team, long *dest, const long *source, size_t nelems);
// typedef int (*shmem_long_alltoalls_func)(shmem_team_t team, long *dest, const long *source, ptrdiff_t dst, ptrdiff_t sst, size_t nelems);
// typedef int (*shmem_long_broadcast_func)(shmem_team_t team, long *dest, const long *source, size_t nelems, int PE_root);
// typedef int (*shmem_long_collect_func)(shmem_team_t team, long *dest, const long *source, size_t nelems);
// typedef int (*shmem_long_fcollect_func)(shmem_team_t team, long *dest, const long *source, size_t nelems);
// typedef int (*shmem_long_and_reduce_func)(shmem_team_t team, long *dest, const long *source, size_t nreduce);
// typedef int (*shmem_long_or_reduce_func)(shmem_team_t team, long *dest, const long *source, size_t nreduce);
// typedef int (*shmem_long_xor_reduce_func)(shmem_team_t team, long *dest, const long *source, size_t nreduce);
// typedef int (*shmem_long_max_reduce_func)(shmem_team_t team, long *dest, const long *source, size_t nreduce);
// typedef int (*shmem_long_min_reduce_func)(shmem_team_t team, long *dest, const long *source, size_t nreduce);
// typedef int (*shmem_long_sum_reduce_func)(shmem_team_t team, long *dest, const long *source, size_t nreduce);
// typedef int (*shmem_long_prod_reduce_func)(shmem_team_t team, long *dest, const long *source, size_t nreduce);

// /* Point-Point Synchronization Routines */
// typedef void (*shmem_long_wait_until_func)(long *ivar, int cmp, long cmp_value);
// typedef void (*shmem_long_wait_until_all_func)(long *ivars, size_t nelems, const int *status, int cmp, long cmp_value);
// typedef size_t (*shmem_long_wait_until_any_func)(long *ivars, size_t nelems, const int *status, int cmp, long cmp_value);
// typedef size_t (*shmem_long_wait_until_some_func)(long *ivars, size_t nelems, size_t *indices, const int *status, int cmp, long cmp_value);
// typedef void (*shmem_long_wait_until_all_vector_func)(long *ivars, size_t nelems, const int *status, int cmp, long *cmp_values);
// typedef size_t (*shmem_long_wait_until_any_vector_func)(long *ivars, size_t nelems, const int *status, int cmp, long *cmp_values);
// typedef size_t (*shmem_long_wait_until_some_vector_func)(long *ivars, size_t nelems, size_t *indices, const int *status, int cmp, long *cmp_values);
// typedef int (*shmem_long_test_func)(long *ivar, int cmp, long cmp_value);
// typedef int (*shmem_long_test_all_func)(long *ivars, size_t nelems, const int *status, int cmp, long cmp_value);
// typedef size_t (*shmem_long_test_any_func)(long *ivars, size_t nelems, const int *status, int cmp,long cmp_value);
// typedef size_t (*shmem_long_test_some_func)(long *ivars, size_t nelems, size_t *indices, const int *status, int cmp, long cmp_value);
// typedef int (*shmem_long_test_all_vector_func)(long *ivars, size_t nelems, const int *status, int cmp, long *cmp_values);
// typedef size_t (*shmem_long_test_any_vector_func)(long *ivars, size_t nelems, const int *status, int cmp, long *cmp_values);
// typedef size_t (*shmem_long_test_some_vector_func)(long *ivars, size_t nelems, size_t *indices, const int *status, int cmp, long *cmp_values);

// typedef uint64_t (*shmem_signal_wait_until_func)(uint64_t *sig_addr, int cmp, uint64_t cmp_value);

// /* Memory Ordering Routines */
// typedef void (*shmem_fence_func)(void);
// typedef void (*shmem_quiet_func)(void);

// /* Distributed Locking Routines */
// typedef void (*shmem_set_lock_func)(long *lock);
// typedef void (*shmem_clear_lock_func)(long *lock);

// }

// /************* Declare global function pointers *************/
// /* Fake routine for testing */
// extern shmem_fake_routine_func p_shmem_fake_routine;

// /* Library Setup, Exit, and Query Routines */
// extern shmem_init_func p_shmem_init;
// extern shmem_init_thread_func p_shmem_init_thread;
// extern shmem_finalize_func p_shmem_finalize;
// extern shmem_my_pe_func p_shmem_my_pe;
// extern shmem_n_pes_func p_shmem_n_pes;
// extern shmem_pe_accessible_func p_shmem_pe_accessible;
// extern shmem_barrier_all_func p_shmem_barrier_all;
// extern shmem_barrier_func p_shmem_barrier;
// extern shmem_info_get_version_func p_shmem_info_get_version;
// extern shmem_info_get_name_func p_shmem_info_get_name;
// extern shmem_global_exit_func p_shmem_global_exit;

// /* Thread Support */
// extern shmem_query_thread_func p_shmem_query_thread;

// /* Memory Management Routines */
// extern shmem_ptr_func p_shmem_ptr;
// extern shmem_malloc_func p_shmem_malloc;
// extern shmem_free_func p_shmem_free;
// extern shmem_realloc_func p_shmem_realloc;
// extern shmem_align_func p_shmem_align;
// extern shmem_malloc_with_hints_func p_shmem_malloc_with_hints;
// extern shmem_calloc_func p_shmem_calloc;
// extern shmem_addr_accessible_func p_shmem_addr_accessible;

// /* Team Management Routines */
// extern shmem_team_my_pe_func p_shmem_team_my_pe;
// extern shmem_team_n_pes_func p_shmem_team_n_pes;
// extern shmem_team_get_config_func p_shmem_team_get_config;
// extern shmem_team_translate_pe_func p_shmem_team_translate_pe;
// extern shmem_team_split_strided_func p_shmem_team_split_strided;
// extern shmem_team_split_2d_func p_shmem_team_split_2d;
// extern shmem_team_destroy_func p_shmem_team_destroy;

// /* Communication/Context Management Routines */
// extern shmem_ctx_create_func p_shmem_ctx_create;
// extern shmem_team_create_ctx_func p_shmem_team_create_ctx;
// extern shmem_ctx_destroy_func p_shmem_ctx_destroy;
// extern shmem_ctx_get_team_func p_shmem_ctx_get_team;

// /* Remote Access Routines */
// extern shmem_long_put_func p_shmem_long_put;
// extern shmem_long_p_func p_shmem_long_p;
// extern shmem_long_iput_func p_shmem_long_iput;
// extern shmem_long_get_func p_shmem_long_get;
// extern shmem_long_g_func p_shmem_long_g;
// extern shmem_long_iget_func p_shmem_long_iget;
// extern shmem_long_put_nbi_func p_shmem_long_put_nbi;
// extern shmem_long_get_nbi_func p_shmem_long_get_nbi;

// /* Atomic Memory Operations */
// extern shmem_long_atomic_fetch_func p_shmem_long_atomic_fetch;
// extern shmem_long_atomic_set_func p_shmem_long_atomic_set;
// extern shmem_long_atomic_compare_swap_func p_shmem_long_atomic_compare_swap;
// extern shmem_long_atomic_swap_func p_shmem_long_atomic_swap;
// extern shmem_long_atomic_fetch_inc_func p_shmem_long_atomic_fetch_inc;
// extern shmem_long_atomic_inc_func p_shmem_long_atomic_inc;
// extern shmem_long_atomic_fetch_add_func p_shmem_long_atomic_fetch_add;
// extern shmem_long_atomic_add_func p_shmem_long_atomic_add;
// extern shmem_long_atomic_fetch_and_func p_shmem_long_atomic_fetch_and;
// extern shmem_long_atomic_and_func p_shmem_long_atomic_and;
// extern shmem_long_atomic_fetch_or_func p_shmem_long_atomic_fetch_or;
// extern shmem_long_atomic_or_func p_shmem_long_atomic_or;
// extern shmem_long_atomic_fetch_xor_func p_shmem_long_atomic_fetch_xor;
// extern shmem_long_atomic_xor_func p_shmem_long_atomic_xor;
// extern shmem_long_atomic_fetch_nbi_func p_shmem_long_atomic_fetch_nbi;
// extern shmem_long_atomic_compare_swap_nbi_func p_shmem_long_atomic_compare_swap_nbi;
// extern shmem_long_atomic_swap_nbi_func p_shmem_long_atomic_swap_nbi;
// extern shmem_long_atomic_fetch_inc_nbi_func p_shmem_long_atomic_fetch_inc_nbi;
// extern shmem_long_atomic_fetch_add_nbi_func p_shmem_long_atomic_fetch_add_nbi;
// extern shmem_long_atomic_fetch_and_nbi_func p_shmem_long_atomic_fetch_and_nbi;
// extern shmem_long_atomic_fetch_or_nbi_func p_shmem_long_atomic_fetch_or_nbi;
// extern shmem_long_atomic_fetch_xor_nbi_func p_shmem_long_atomic_fetch_xor_nbi;

// /* Signaling Operations */
// extern shmem_long_put_signal_func p_shmem_long_put_signal;
// extern shmem_long_put_signal_nbi_func p_shmem_long_put_signal_nbi;
// extern shmem_signal_fetch_func p_shmem_signal_fetch;

// /* Collective Routines */
// extern shmem_sync_func p_shmem_sync;
// extern shmem_sync_all_func p_shmem_sync_all;
// extern shmem_long_alltoall_func p_shmem_long_alltoall;
// extern shmem_long_alltoalls_func p_shmem_long_alltoalls;
// extern shmem_long_broadcast_func p_shmem_long_broadcast;
// extern shmem_long_collect_func p_shmem_long_collect;
// extern shmem_long_fcollect_func p_shmem_long_fcollect;
// extern shmem_long_and_reduce_func p_shmem_long_and_reduce;
// extern shmem_long_or_reduce_func p_shmem_long_or_reduce;
// extern shmem_long_xor_reduce_func p_shmem_long_xor_reduce;
// extern shmem_long_max_reduce_func p_shmem_long_max_reduce;
// extern shmem_long_min_reduce_func p_shmem_long_min_reduce;
// extern shmem_long_sum_reduce_func p_shmem_long_sum_reduce;
// extern shmem_long_prod_reduce_func p_shmem_long_prod_reduce;

// /* Point-Point Synchronization Routines */
// extern shmem_long_wait_until_func p_shmem_long_wait_until;
// extern shmem_long_wait_until_all_func p_shmem_long_wait_until_all;
// extern shmem_long_wait_until_any_func p_shmem_long_wait_until_any;
// extern shmem_long_wait_until_some_func p_shmem_long_wait_until_some;
// extern shmem_long_wait_until_all_vector_func p_shmem_long_wait_until_all_vector;
// extern shmem_long_wait_until_any_vector_func p_shmem_long_wait_until_any_vector;
// extern shmem_long_wait_until_some_vector_func p_shmem_long_wait_until_some_vector;
// extern shmem_long_test_func p_shmem_long_test;
// extern shmem_long_test_all_func p_shmem_long_test_all;
// extern shmem_long_test_any_func p_shmem_long_test_any;
// extern shmem_long_test_some_func p_shmem_long_test_some;
// extern shmem_long_test_all_vector_func p_shmem_long_test_all_vector;
// extern shmem_long_test_any_vector_func p_shmem_long_test_any_vector;
// extern shmem_long_test_some_vector_func p_shmem_long_test_some_vector;
// extern shmem_signal_wait_until_func p_shmem_signal_wait_until;

// /* Memory Ordering Routines */
// extern shmem_fence_func p_shmem_fence;
// extern shmem_quiet_func p_shmem_quiet;

// /* Distributed Locking Routines */
// extern shmem_set_lock_func p_shmem_set_lock;
// extern shmem_clear_lock_func p_shmem_clear_lock;




/**
  @file routines.hpp
  @brief Contains routine pointers to avoid compiler errors if a routine is not
         implemented
 */

#pragma once

#include <shmem.h> /* Include OpenSHMEM header for types and constants */

/************* Define function pointers for OpenSHMEM routines *************/

extern "C" {

/* Fake routine for testing */
typedef void (*shmem_fake_routine_func)(void);

/* Library Setup, Exit, and Query Routines */
typedef void (*shmem_init_func)(void);
typedef void (*shmem_finalize_func)(void);
typedef int (*shmem_my_pe_func)(void);
typedef int (*shmem_n_pes_func)(void);
typedef int (*shmem_pe_accessible_func)(int pe);
typedef void (*shmem_barrier_all_func)(void);
typedef void (*shmem_barrier_func)(int PE_start, int logPE_stride, int PE_size, long *pSync);
typedef void (*shmem_info_get_version_func)(int *major, int *minor);
typedef void (*shmem_info_get_name_func)(char *name);
typedef void (*shmem_global_exit_func)(int status);

/* Thread Support */
typedef int (*shmem_init_thread_func)(int requested, int *provided);
typedef int (*shmem_query_thread_func)(int *provided);

/* Memory Management Routines */
typedef void* (*shmem_ptr_func)(const void *dest, int pe);
typedef void* (*shmem_malloc_func)(size_t size);
typedef void (*shmem_free_func)(void *ptr);
typedef void* (*shmem_realloc_func)(void *ptr, size_t size);
typedef void* (*shmem_align_func)(size_t alignment, size_t size);
typedef void* (*shmem_malloc_with_hints_func)(size_t size, long hints);
typedef void* (*shmem_calloc_func)(size_t count, size_t size);
typedef int (*shmem_addr_accessible_func)(const void *addr, int pe);

/* Team Management Routines */
typedef int (*shmem_team_my_pe_func)(shmem_team_t team);
typedef int (*shmem_team_n_pes_func)(shmem_team_t team);
typedef void (*shmem_team_get_config_func)(shmem_team_t team, shmem_team_config_t *config);
typedef int (*shmem_team_translate_pe_func)(shmem_team_t src_team, int src_pe, shmem_team_t dest_team);
typedef shmem_team_t (*shmem_team_split_strided_func)(shmem_team_t parent_team, int start, int stride, int size, const shmem_team_config_t *config, long config_mask, shmem_team_t *new_team);
typedef shmem_team_t (*shmem_team_split_2d_func)(shmem_team_t parent_team, int xrange, const shmem_team_config_t *xaxis_config, long xaxis_mask, shmem_team_t *xaxis_team, const shmem_team_config_t *yaxis_config, long yaxis_mask, shmem_team_t *yaxis_team);
typedef void (*shmem_team_destroy_func)(shmem_team_t team);

/* Communication/Context Management Routines */
typedef int (*shmem_ctx_create_func)(long options, shmem_ctx_t *ctx);
typedef int (*shmem_team_create_ctx_func)(shmem_team_t team, long options, shmem_ctx_t *ctx);
typedef void (*shmem_ctx_destroy_func)(shmem_ctx_t ctx);
typedef int (*shmem_ctx_get_team_func)(shmem_ctx_t ctx, shmem_team_t *team);

/* Remote Access Routines */
typedef void (*shmem_long_put_func)(long *dest, const long *src, size_t nelems, int pe);
typedef void (*shmem_long_p_func)(long *dest, long value, int pe);
typedef void (*shmem_long_iput_func)(long *dest, const long *src, ptrdiff_t tst, ptrdiff_t sst, size_t nelems, int pe);
typedef void (*shmem_long_get_func)(long *dest, const long *src, size_t nelems, int pe);
typedef long (*shmem_long_g_func)(const long *src, int pe);
typedef void (*shmem_long_iget_func)(long *dest, const long *src, ptrdiff_t tst, ptrdiff_t sst, size_t nelems, int pe);
typedef void (*shmem_long_put_nbi_func)(long *dest, const long *src, size_t nelems, int pe);
typedef void (*shmem_long_get_nbi_func)(long *dest, const long *src, size_t nelems, int pe);

/* Atomic Memory Operations */
typedef long (*shmem_long_atomic_fetch_func)(const long *target, int pe);
typedef void (*shmem_long_atomic_set_func)(long *target, long value, int pe);
typedef long (*shmem_long_atomic_compare_swap_func)(long *target, long cond, long value, int pe);
typedef long (*shmem_long_atomic_swap_func)(long *target, long value, int pe);
typedef long (*shmem_long_atomic_fetch_inc_func)(const long *target, int pe);
typedef void (*shmem_long_atomic_inc_func)(long *target, int pe);
typedef long (*shmem_long_atomic_fetch_add_func)(const long *target, long value, int pe);
typedef void (*shmem_long_atomic_add_func)(const long *target, long value, int pe);
typedef long (*shmem_long_atomic_fetch_and_func)(const long *target, long value, int pe);
typedef void (*shmem_long_atomic_and_func)(const long *target, long value, int pe);
typedef long (*shmem_long_atomic_fetch_or_func)(const long *target, long value, int pe);
typedef void (*shmem_long_atomic_or_func)(const long *target, long value, int pe);
typedef long (*shmem_long_atomic_fetch_xor_func)(const long *target, long value, int pe);
typedef void (*shmem_long_atomic_xor_func)(const long *target, long value, int pe);
typedef void (*shmem_long_atomic_fetch_nbi_func)(long *dest, const long *target, int pe);
typedef void (*shmem_long_atomic_compare_swap_nbi_func)(long *dest, long *target, long cond, long value, int pe);
typedef void (*shmem_long_atomic_swap_nbi_func)(long *dest, long *target, long value, int pe);
typedef void (*shmem_long_atomic_fetch_inc_nbi_func)(long *dest, const long *target, int pe);
typedef void (*shmem_long_atomic_fetch_add_nbi_func)(long *dest, const long *target, long value, int pe);
typedef void (*shmem_long_atomic_fetch_and_nbi_func)(long *dest, const long *target, long value, int pe);
typedef void (*shmem_long_atomic_fetch_or_nbi_func)(long *dest, const long *target, long value, int pe);
typedef void (*shmem_long_atomic_fetch_xor_nbi_func)(long *dest, const long *target, long value, int pe);

/* Signaling Operations */
typedef void (*shmem_long_put_signal_func)(long *dest, const long *source, size_t nelems, uint64_t *sig_addr, uint64_t signal, int sig_op, int pe);
typedef void (*shmem_long_put_signal_nbi_func)(long *dest, const long *source, size_t nelems, uint64_t *sig_addr, uint64_t signal, int sig_op, int pe);
typedef long (*shmem_signal_fetch_func)(const uint64_t *sig_addr);

/* Collective Routines */
typedef int (*shmem_sync_func)(int PE_start, int logPE_stride, int PE_size, long *pSync);
typedef void (*shmem_sync_all_func)(void);
typedef int (*shmem_long_alltoall_func)(shmem_team_t team, long *dest, const long *source, size_t nelems);
typedef int (*shmem_long_alltoalls_func)(shmem_team_t team, long *dest, const long *source, ptrdiff_t dst, ptrdiff_t sst, size_t nelems);
typedef int (*shmem_long_broadcast_func)(shmem_team_t team, long *dest, const long *source, size_t nelems, int PE_root);
typedef int (*shmem_long_collect_func)(shmem_team_t team, long *dest, const long *source, size_t nelems);
typedef int (*shmem_long_fcollect_func)(shmem_team_t team, long *dest, const long *source, size_t nelems);
typedef int (*shmem_long_and_reduce_func)(shmem_team_t team, long *dest, const long *source, size_t nreduce);
typedef int (*shmem_long_or_reduce_func)(shmem_team_t team, long *dest, const long *source, size_t nreduce);
typedef int (*shmem_long_xor_reduce_func)(shmem_team_t team, long *dest, const long *source, size_t nreduce);
typedef int (*shmem_long_max_reduce_func)(shmem_team_t team, long *dest, const long *source, size_t nreduce);
typedef int (*shmem_long_min_reduce_func)(shmem_team_t team, long *dest, const long *source, size_t nreduce);
typedef int (*shmem_long_sum_reduce_func)(shmem_team_t team, long *dest, const long *source, size_t nreduce);
typedef int (*shmem_long_prod_reduce_func)(shmem_team_t team, long *dest, const long *source, size_t nreduce);

/* Point-Point Synchronization Routines */
typedef void (*shmem_long_wait_until_func)(long *ivar, int cmp, long cmp_value);
typedef void (*shmem_long_wait_until_all_func)(long *ivars, size_t nelems, const int *status, int cmp, long cmp_value);
typedef size_t (*shmem_long_wait_until_any_func)(long *ivars, size_t nelems, const int *status, int cmp, long cmp_value);
typedef size_t (*shmem_long_wait_until_some_func)(long *ivars, size_t nelems, size_t *indices, const int *status, int cmp, long cmp_value);
typedef void (*shmem_long_wait_until_all_vector_func)(long *ivars, size_t nelems, const int *status, int cmp, long *cmp_values);
typedef size_t (*shmem_long_wait_until_any_vector_func)(long *ivars, size_t nelems, const int *status, int cmp, long *cmp_values);
typedef size_t (*shmem_long_wait_until_some_vector_func)(long *ivars, size_t nelems, size_t *indices, const int *status, int cmp, long *cmp_values);
typedef int (*shmem_long_test_func)(long *ivar, int cmp, long cmp_value);
typedef int (*shmem_long_test_all_func)(long *ivars, size_t nelems, const int *status, int cmp, long cmp_value);
typedef size_t (*shmem_long_test_any_func)(long *ivars, size_t nelems, const int *status, int cmp,long cmp_value);
typedef size_t (*shmem_long_test_some_func)(long *ivars, size_t nelems, size_t *indices, const int *status, int cmp, long cmp_value);
typedef int (*shmem_long_test_all_vector_func)(long *ivars, size_t nelems, const int *status, int cmp, long *cmp_values);
typedef size_t (*shmem_long_test_any_vector_func)(long *ivars, size_t nelems, const int *status, int cmp, long *cmp_values);
typedef size_t (*shmem_long_test_some_vector_func)(long *ivars, size_t nelems, size_t *indices, const int *status, int cmp, long *cmp_values);

typedef uint64_t (*shmem_signal_wait_until_func)(uint64_t *sig_addr, int cmp, uint64_t cmp_value);

/* Memory Ordering Routines */
typedef void (*shmem_fence_func)(void);
typedef void (*shmem_quiet_func)(void);

/* Distributed Locking Routines */
typedef void (*shmem_set_lock_func)(long *lock);
typedef void (*shmem_clear_lock_func)(long *lock);

}

/************* Declare global function pointers *************/
/* Fake routine for testing */
extern shmem_fake_routine_func p_shmem_fake_routine;

/* Library Setup, Exit, and Query Routines */
extern shmem_init_func p_shmem_init;
extern shmem_init_thread_func p_shmem_init_thread;
extern shmem_finalize_func p_shmem_finalize;
extern shmem_my_pe_func p_shmem_my_pe;
extern shmem_n_pes_func p_shmem_n_pes;
extern shmem_pe_accessible_func p_shmem_pe_accessible;
extern shmem_barrier_all_func p_shmem_barrier_all;
extern shmem_barrier_func p_shmem_barrier;
extern shmem_info_get_version_func p_shmem_info_get_version;
extern shmem_info_get_name_func p_shmem_info_get_name;
extern shmem_global_exit_func p_shmem_global_exit;

/* Thread Support */
extern shmem_query_thread_func p_shmem_query_thread;

/* Memory Management Routines */
extern shmem_ptr_func p_shmem_ptr;
extern shmem_malloc_func p_shmem_malloc;
extern shmem_free_func p_shmem_free;
extern shmem_realloc_func p_shmem_realloc;
extern shmem_align_func p_shmem_align;
extern shmem_malloc_with_hints_func p_shmem_malloc_with_hints;
extern shmem_calloc_func p_shmem_calloc;
extern shmem_addr_accessible_func p_shmem_addr_accessible;

/* Team Management Routines */
extern shmem_team_my_pe_func p_shmem_team_my_pe;
extern shmem_team_n_pes_func p_shmem_team_n_pes;
extern shmem_team_get_config_func p_shmem_team_get_config;
extern shmem_team_translate_pe_func p_shmem_team_translate_pe;
extern shmem_team_split_strided_func p_shmem_team_split_strided;
extern shmem_team_split_2d_func p_shmem_team_split_2d;
extern shmem_team_destroy_func p_shmem_team_destroy;

/* Communication/Context Management Routines */
extern shmem_ctx_create_func p_shmem_ctx_create;
extern shmem_team_create_ctx_func p_shmem_team_create_ctx;
extern shmem_ctx_destroy_func p_shmem_ctx_destroy;
extern shmem_ctx_get_team_func p_shmem_ctx_get_team;

/* Remote Access Routines */
extern shmem_long_put_func p_shmem_long_put;
extern shmem_long_p_func p_shmem_long_p;
extern shmem_long_iput_func p_shmem_long_iput;
extern shmem_long_get_func p_shmem_long_get;
extern shmem_long_g_func p_shmem_long_g;
extern shmem_long_iget_func p_shmem_long_iget;
extern shmem_long_put_nbi_func p_shmem_long_put_nbi;
extern shmem_long_get_nbi_func p_shmem_long_get_nbi;

/* Atomic Memory Operations */
extern shmem_long_atomic_fetch_func p_shmem_long_atomic_fetch;
extern shmem_long_atomic_set_func p_shmem_long_atomic_set;
extern shmem_long_atomic_compare_swap_func p_shmem_long_atomic_compare_swap;
extern shmem_long_atomic_swap_func p_shmem_long_atomic_swap;
extern shmem_long_atomic_fetch_inc_func p_shmem_long_atomic_fetch_inc;
extern shmem_long_atomic_inc_func p_shmem_long_atomic_inc;
extern shmem_long_atomic_fetch_add_func p_shmem_long_atomic_fetch_add;
extern shmem_long_atomic_add_func p_shmem_long_atomic_add;
extern shmem_long_atomic_fetch_and_func p_shmem_long_atomic_fetch_and;
extern shmem_long_atomic_and_func p_shmem_long_atomic_and;
extern shmem_long_atomic_fetch_or_func p_shmem_long_atomic_fetch_or;
extern shmem_long_atomic_or_func p_shmem_long_atomic_or;
extern shmem_long_atomic_fetch_xor_func p_shmem_long_atomic_fetch_xor;
extern shmem_long_atomic_xor_func p_shmem_long_atomic_xor;
extern shmem_long_atomic_fetch_nbi_func p_shmem_long_atomic_fetch_nbi;
extern shmem_long_atomic_compare_swap_nbi_func p_shmem_long_atomic_compare_swap_nbi;
extern shmem_long_atomic_swap_nbi_func p_shmem_long_atomic_swap_nbi;
extern shmem_long_atomic_fetch_inc_nbi_func p_shmem_long_atomic_fetch_inc_nbi;
extern shmem_long_atomic_fetch_add_nbi_func p_shmem_long_atomic_fetch_add_nbi;
extern shmem_long_atomic_fetch_and_nbi_func p_shmem_long_atomic_fetch_and_nbi;
extern shmem_long_atomic_fetch_or_nbi_func p_shmem_long_atomic_fetch_or_nbi;
extern shmem_long_atomic_fetch_xor_nbi_func p_shmem_long_atomic_fetch_xor_nbi;

/* Signaling Operations */
extern shmem_long_put_signal_func p_shmem_long_put_signal;
extern shmem_long_put_signal_nbi_func p_shmem_long_put_signal_nbi;
extern shmem_signal_fetch_func p_shmem_signal_fetch;

/* Collective Routines */
extern shmem_sync_func p_shmem_sync;
extern shmem_sync_all_func p_shmem_sync_all;
extern shmem_long_alltoall_func p_shmem_long_alltoall;
extern shmem_long_alltoalls_func p_shmem_long_alltoalls;
extern shmem_long_broadcast_func p_shmem_long_broadcast;
extern shmem_long_collect_func p_shmem_long_collect;
extern shmem_long_fcollect_func p_shmem_long_fcollect;
extern shmem_long_and_reduce_func p_shmem_long_and_reduce;
extern shmem_long_or_reduce_func p_shmem_long_or_reduce;
extern shmem_long_xor_reduce_func p_shmem_long_xor_reduce;
extern shmem_long_max_reduce_func p_shmem_long_max_reduce;
extern shmem_long_min_reduce_func p_shmem_long_min_reduce;
extern shmem_long_sum_reduce_func p_shmem_long_sum_reduce;
extern shmem_long_prod_reduce_func p_shmem_long_prod_reduce;

/* Point-Point Synchronization Routines */
extern shmem_long_wait_until_func p_shmem_long_wait_until;
extern shmem_long_wait_until_all_func p_shmem_long_wait_until_all;
extern shmem_long_wait_until_any_func p_shmem_long_wait_until_any;
extern shmem_long_wait_until_some_func p_shmem_long_wait_until_some;
extern shmem_long_wait_until_all_vector_func p_shmem_long_wait_until_all_vector;
extern shmem_long_wait_until_any_vector_func p_shmem_long_wait_until_any_vector;
extern shmem_long_wait_until_some_vector_func p_shmem_long_wait_until_some_vector;
extern shmem_long_test_func p_shmem_long_test;
extern shmem_long_test_all_func p_shmem_long_test_all;
extern shmem_long_test_any_func p_shmem_long_test_any;
extern shmem_long_test_some_func p_shmem_long_test_some;
extern shmem_long_test_all_vector_func p_shmem_long_test_all_vector;
extern shmem_long_test_any_vector_func p_shmem_long_test_any_vector;
extern shmem_long_test_some_vector_func p_shmem_long_test_some_vector;
extern shmem_signal_wait_until_func p_shmem_signal_wait_until;

/* Memory Ordering Routines */
extern shmem_quiet_func p_shmem_quiet;
extern shmem_fence_func p_shmem_fence;

/* Distributed Locking Routines */
extern shmem_set_lock_func p_shmem_set_lock;
extern shmem_clear_lock_func p_shmem_clear_lock;
