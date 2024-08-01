// /**
//  * @file routines.cpp
//  * @brief Contains function pointer declarations and routine loading function for the OpenSHMEM library.
//  */

// #include "routines.h"

// #include <iostream>
// #include <dlfcn.h>

// /* Define global function pointers */
// shmem_fake_routine_func p_shmem_fake_routine = nullptr;

// /* Setup, Exit, and Query Routines */
// shmem_init_func p_shmem_init = nullptr;
// shmem_finalize_func p_shmem_finalize = nullptr;
// shmem_my_pe_func p_shmem_my_pe = nullptr;
// shmem_n_pes_func p_shmem_n_pes = nullptr;
// shmem_pe_accessible_func p_shmem_pe_accessible = nullptr;
// shmem_barrier_all_func p_shmem_barrier_all = nullptr;
// shmem_barrier_func p_shmem_barrier = nullptr;
// shmem_info_get_version_func p_shmem_info_get_version = nullptr;
// shmem_info_get_name_func p_shmem_info_get_name = nullptr;
// shmem_global_exit_func p_shmem_global_exit = nullptr;

// /* Thread Support Routines */
// shmem_init_thread_func p_shmem_init_thread = nullptr;
// shmem_query_thread_func p_shmem_query_thread = nullptr;

// /* Memory Management Routines */
// shmem_ptr_func p_shmem_ptr = nullptr;
// shmem_malloc_func p_shmem_malloc = nullptr;
// shmem_free_func p_shmem_free = nullptr;
// shmem_realloc_func p_shmem_realloc = nullptr;
// shmem_align_func p_shmem_align = nullptr;
// shmem_malloc_with_hints_func p_shmem_malloc_with_hints = nullptr;
// shmem_calloc_func p_shmem_calloc = nullptr;
// shmem_addr_accessible_func p_shmem_addr_accessible = nullptr;

// /* Team Management Routines */
// shmem_team_my_pe_func p_shmem_team_my_pe = nullptr;
// shmem_team_n_pes_func p_shmem_team_n_pes = nullptr;
// shmem_team_get_config_func p_shmem_team_get_config = nullptr;
// shmem_team_translate_pe_func p_shmem_team_translate_pe = nullptr;
// shmem_team_split_strided_func p_shmem_team_split_strided = nullptr;
// shmem_team_split_2d_func p_shmem_team_split_2d = nullptr;
// shmem_team_destroy_func p_shmem_team_destroy = nullptr;

// /* Communication/Context Management Routines */
// shmem_ctx_create_func p_shmem_ctx_create = nullptr;
// shmem_team_create_ctx_func p_shmem_team_create_ctx = nullptr;
// shmem_ctx_destroy_func p_shmem_ctx_destroy = nullptr;
// shmem_ctx_get_team_func p_shmem_ctx_get_team = nullptr;

// /* Remote Access Routines */
// shmem_long_put_func p_shmem_long_put = nullptr;
// shmem_long_p_func p_shmem_long_p = nullptr;
// shmem_long_iput_func p_shmem_long_iput = nullptr;
// shmem_long_get_func p_shmem_long_get = nullptr;
// shmem_long_g_func p_shmem_long_g = nullptr;
// shmem_long_iget_func p_shmem_long_iget = nullptr;
// shmem_long_put_nbi_func p_shmem_long_put_nbi = nullptr;
// shmem_long_get_nbi_func p_shmem_long_get_nbi = nullptr;


// /* Atomic Memory Operations */
// shmem_ulong_atomic_fetch_func p_shmem_ulong_atomic_fetch = nullptr;
// shmem_ulong_atomic_set_func p_shmem_ulong_atomic_set = nullptr;
// shmem_ulong_atomic_compare_swap_func p_shmem_ulong_atomic_compare_swap = nullptr;
// shmem_ulong_atomic_swap_func p_shmem_ulong_atomic_swap = nullptr;
// shmem_ulong_atomic_fetch_inc_func p_shmem_ulong_atomic_fetch_inc = nullptr;
// shmem_ulong_atomic_inc_func p_shmem_ulong_atomic_inc = nullptr;
// shmem_ulong_atomic_fetch_add_func p_shmem_ulong_atomic_fetch_add = nullptr;
// shmem_ulong_atomic_add_func p_shmem_ulong_atomic_add = nullptr;
// shmem_ulong_atomic_fetch_and_func p_shmem_ulong_atomic_fetch_and = nullptr;
// shmem_ulong_atomic_and_func p_shmem_ulong_atomic_and = nullptr;
// shmem_ulong_atomic_fetch_or_func p_shmem_ulong_atomic_fetch_or = nullptr;
// shmem_ulong_atomic_or_func p_shmem_ulong_atomic_or = nullptr;
// shmem_ulong_atomic_fetch_xor_func p_shmem_ulong_atomic_fetch_xor = nullptr;
// shmem_ulong_atomic_xor_func p_shmem_ulong_atomic_xor = nullptr;
// shmem_ulong_atomic_fetch_nbi_func p_shmem_ulong_atomic_fetch_nbi = nullptr;
// shmem_ulong_atomic_compare_swap_nbi_func p_shmem_ulong_atomic_compare_swap_nbi = nullptr;
// shmem_ulong_atomic_swap_nbi_func p_shmem_ulong_atomic_swap_nbi = nullptr;
// shmem_ulong_atomic_fetch_inc_nbi_func p_shmem_ulong_atomic_fetch_inc_nbi = nullptr;
// shmem_ulong_atomic_fetch_add_nbi_func p_shmem_ulong_atomic_fetch_add_nbi = nullptr;
// shmem_ulong_atomic_fetch_and_nbi_func p_shmem_ulong_atomic_fetch_and_nbi = nullptr;
// shmem_ulong_atomic_fetch_or_nbi_func p_shmem_ulong_atomic_fetch_or_nbi = nullptr;
// shmem_ulong_atomic_fetch_xor_nbi_func p_shmem_ulong_atomic_fetch_xor_nbi = nullptr;

// /* Signaling Operations */
// shmem_signal_fetch_func p_shmem_signal_fetch = nullptr;
// shmem_long_put_signal_func p_shmem_long_put_signal = nullptr;
// shmem_long_put_signal_nbi_func p_shmem_long_put_signal_nbi = nullptr;

// /* Collective Routines */
// shmem_sync_func p_shmem_sync = nullptr;
// shmem_sync_all_func p_shmem_sync_all = nullptr;
// shmem_long_alltoall_func p_shmem_long_alltoall = nullptr;
// shmem_long_alltoalls_func p_shmem_long_alltoalls = nullptr;
// shmem_long_broadcast_func p_shmem_long_broadcast = nullptr;
// shmem_long_collect_func p_shmem_long_collect = nullptr;
// shmem_long_fcollect_func p_shmem_long_fcollect = nullptr;
// shmem_long_and_reduce_func p_shmem_long_and_reduce = nullptr;
// shmem_long_or_reduce_func p_shmem_long_or_reduce = nullptr;
// shmem_long_xor_reduce_func p_shmem_long_xor_reduce = nullptr;
// shmem_long_max_reduce_func p_shmem_long_max_reduce = nullptr;
// shmem_long_min_reduce_func p_shmem_long_min_reduce = nullptr;
// shmem_long_sum_reduce_func p_shmem_long_sum_reduce = nullptr;
// shmem_long_prod_reduce_func p_shmem_long_prod_reduce = nullptr;

// /* Point-to-Point Synchronization Routines */
// shmem_long_wait_until_func p_shmem_long_wait_until = nullptr;
// shmem_long_wait_until_all_func p_shmem_long_wait_until_all = nullptr;
// shmem_long_wait_until_any_func p_shmem_long_wait_until_any = nullptr;
// shmem_long_wait_until_some_func p_shmem_long_wait_until_some = nullptr;
// shmem_long_wait_until_all_vector_func p_shmem_long_wait_until_all_vector = nullptr;
// shmem_long_wait_until_any_vector_func p_shmem_long_wait_until_any_vector = nullptr;
// shmem_long_wait_until_some_vector_func p_shmem_long_wait_until_some_vector = nullptr;
// shmem_long_test_func p_shmem_long_test = nullptr;
// shmem_long_test_all_func p_shmem_long_test_all = nullptr;
// shmem_long_test_any_func p_shmem_long_test_any = nullptr;
// shmem_long_test_some_func p_shmem_long_test_some = nullptr;
// shmem_long_test_all_vector_func p_shmem_long_test_all_vector = nullptr;
// shmem_long_test_any_vector_func p_shmem_long_test_any_vector = nullptr;
// shmem_long_test_some_vector_func p_shmem_long_test_some_vector = nullptr;
// shmem_signal_wait_until_func p_shmem_signal_wait_until = nullptr;

// /* Memory Ordering Routines */
// shmem_quiet_func p_shmem_quiet = nullptr;
// shmem_fence_func p_shmem_fence = nullptr;

// /* Distributed Locking Routines */
// shmem_set_lock_func p_shmem_set_lock = nullptr;
// shmem_clear_lock_func p_shmem_clear_lock = nullptr;

// /**
//  * @brief Loads the OpenSHMEM routines dynamically.
//  *
//  * This function loads the OpenSHMEM routines at runtime using dynamic linking.
//  *
//  * @return True if successful, false otherwise.
//  */
// bool load_routines() {
//   void *handle = dlopen(NULL, RTLD_LAZY);
//   if (!handle) {
//     std::cerr << "Failed to open handle: " << dlerror() << std::endl;
//     return false;
//   }

//   p_shmem_fake_routine = reinterpret_cast<shmem_fake_routine_func>(dlsym(handle, "shmem_fake_routine"));

//   /* Setup, Exit, and Query Routines */
//   p_shmem_init = reinterpret_cast<shmem_init_func>(dlsym(handle, "shmem_init"));
//   p_shmem_finalize = reinterpret_cast<shmem_finalize_func>(dlsym(handle, "shmem_finalize"));
//   p_shmem_my_pe = reinterpret_cast<shmem_my_pe_func>(dlsym(handle, "shmem_my_pe"));
//   p_shmem_n_pes = reinterpret_cast<shmem_n_pes_func>(dlsym(handle, "shmem_n_pes"));
//   p_shmem_pe_accessible = reinterpret_cast<shmem_pe_accessible_func>(dlsym(handle, "shmem_pe_accessible"));
//   p_shmem_barrier_all = reinterpret_cast<shmem_barrier_all_func>(dlsym(handle, "shmem_barrier_all"));
//   p_shmem_barrier = reinterpret_cast<shmem_barrier_func>(dlsym(handle, "shmem_barrier"));
//   p_shmem_info_get_version = reinterpret_cast<shmem_info_get_version_func>(dlsym(handle, "shmem_info_get_version"));
//   p_shmem_info_get_name = reinterpret_cast<shmem_info_get_name_func>(dlsym(handle, "shmem_info_get_name"));
//   p_shmem_global_exit = reinterpret_cast<shmem_global_exit_func>(dlsym(handle, "shmem_global_exit"));

//   /* Thread Support Routines */
//   p_shmem_init_thread = reinterpret_cast<shmem_init_thread_func>(dlsym(handle, "shmem_init_thread"));
//   p_shmem_query_thread = reinterpret_cast<shmem_query_thread_func>(dlsym(handle, "shmem_query_thread"));

//   /* Memory Management Routines */
//   p_shmem_ptr = reinterpret_cast<shmem_ptr_func>(dlsym(handle, "shmem_ptr"));
//   p_shmem_malloc = reinterpret_cast<shmem_malloc_func>(dlsym(handle, "shmem_malloc"));
//   p_shmem_free = reinterpret_cast<shmem_free_func>(dlsym(handle, "shmem_free"));
//   p_shmem_realloc = reinterpret_cast<shmem_realloc_func>(dlsym(handle, "shmem_realloc"));
//   p_shmem_align = reinterpret_cast<shmem_align_func>(dlsym(handle, "shmem_align"));
//   p_shmem_malloc_with_hints = reinterpret_cast<shmem_malloc_with_hints_func>(dlsym(handle, "shmem_malloc_with_hints"));
//   p_shmem_calloc = reinterpret_cast<shmem_calloc_func>(dlsym(handle, "shmem_calloc"));
//   p_shmem_addr_accessible = reinterpret_cast<shmem_addr_accessible_func>(dlsym(handle, "shmem_addr_accessible"));

//   /* Team Management Routines */
//   p_shmem_team_my_pe = reinterpret_cast<shmem_team_my_pe_func>(dlsym(handle, "shmem_team_my_pe"));
//   p_shmem_team_n_pes = reinterpret_cast<shmem_team_n_pes_func>(dlsym(handle, "shmem_team_n_pes"));
//   p_shmem_team_get_config = reinterpret_cast<shmem_team_get_config_func>(dlsym(handle, "shmem_team_get_config"));
//   p_shmem_team_translate_pe = reinterpret_cast<shmem_team_translate_pe_func>(dlsym(handle, "shmem_team_translate_pe"));
//   p_shmem_team_split_strided = reinterpret_cast<shmem_team_split_strided_func>(dlsym(handle, "shmem_team_split_strided"));
//   p_shmem_team_split_2d = reinterpret_cast<shmem_team_split_2d_func>(dlsym(handle, "shmem_team_split_2d"));
//   p_shmem_team_destroy = reinterpret_cast<shmem_team_destroy_func>(dlsym(handle, "shmem_team_destroy"));

//   /* Communication/Context Management Routines */
//   p_shmem_ctx_create = reinterpret_cast<shmem_ctx_create_func>(dlsym(handle, "shmem_ctx_create"));
//   p_shmem_team_create_ctx = reinterpret_cast<shmem_team_create_ctx_func>(dlsym(handle, "shmem_team_create_ctx"));
//   p_shmem_ctx_destroy = reinterpret_cast<shmem_ctx_destroy_func>(dlsym(handle, "shmem_ctx_destroy"));
//   p_shmem_ctx_get_team = reinterpret_cast<shmem_ctx_get_team_func>(dlsym(handle, "shmem_ctx_get_team"));

//   /* Remote Access Routines */
//   p_shmem_long_put = reinterpret_cast<shmem_long_put_func>(dlsym(handle, "shmem_long_put"));
//   p_shmem_long_p = reinterpret_cast<shmem_long_p_func>(dlsym(handle, "shmem_long_p"));
//   p_shmem_long_iput = reinterpret_cast<shmem_long_iput_func>(dlsym(handle, "shmem_long_iput"));
//   p_shmem_long_get = reinterpret_cast<shmem_long_get_func>(dlsym(handle, "shmem_long_get"));
//   p_shmem_long_g = reinterpret_cast<shmem_long_g_func>(dlsym(handle, "shmem_long_g"));
//   p_shmem_long_iget = reinterpret_cast<shmem_long_iget_func>(dlsym(handle, "shmem_long_iget"));
//   p_shmem_long_put_nbi = reinterpret_cast<shmem_long_put_nbi_func>(dlsym(handle, "shmem_long_put_nbi"));
//   p_shmem_long_get_nbi = reinterpret_cast<shmem_long_get_nbi_func>(dlsym(handle, "shmem_long_get_nbi"));

//   /* Atomic Memory Operations */
//   p_shmem_ulong_atomic_fetch = reinterpret_cast<shmem_ulong_atomic_fetch_func>(dlsym(handle, "shmem_ulong_atomic_fetch"));
//   p_shmem_ulong_atomic_set = reinterpret_cast<shmem_ulong_atomic_set_func>(dlsym(handle, "shmem_ulong_atomic_set"));
//   p_shmem_ulong_atomic_compare_swap = reinterpret_cast<shmem_ulong_atomic_compare_swap_func>(dlsym(handle, "shmem_ulong_atomic_compare_swap"));
//   p_shmem_ulong_atomic_swap = reinterpret_cast<shmem_ulong_atomic_swap_func>(dlsym(handle, "shmem_ulong_atomic_swap"));
//   p_shmem_ulong_atomic_fetch_inc = reinterpret_cast<shmem_ulong_atomic_fetch_inc_func>(dlsym(handle, "shmem_ulong_atomic_fetch_inc"));
//   p_shmem_ulong_atomic_inc = reinterpret_cast<shmem_ulong_atomic_inc_func>(dlsym(handle, "shmem_ulong_atomic_inc"));
//   p_shmem_ulong_atomic_fetch_add = reinterpret_cast<shmem_ulong_atomic_fetch_add_func>(dlsym(handle, "shmem_ulong_atomic_fetch_add"));
//   p_shmem_ulong_atomic_add = reinterpret_cast<shmem_ulong_atomic_add_func>(dlsym(handle, "shmem_ulong_atomic_add"));
//   p_shmem_ulong_atomic_fetch_and = reinterpret_cast<shmem_ulong_atomic_fetch_and_func>(dlsym(handle, "shmem_ulong_atomic_fetch_and"));
//   p_shmem_ulong_atomic_and = reinterpret_cast<shmem_ulong_atomic_and_func>(dlsym(handle, "shmem_ulong_atomic_and"));
//   p_shmem_ulong_atomic_fetch_or = reinterpret_cast<shmem_ulong_atomic_fetch_or_func>(dlsym(handle, "shmem_ulong_atomic_fetch_or"));
//   p_shmem_ulong_atomic_or = reinterpret_cast<shmem_ulong_atomic_or_func>(dlsym(handle, "shmem_ulong_atomic_or"));
//   p_shmem_ulong_atomic_fetch_xor = reinterpret_cast<shmem_ulong_atomic_fetch_xor_func>(dlsym(handle, "shmem_ulong_atomic_fetch_xor"));
//   p_shmem_ulong_atomic_xor = reinterpret_cast<shmem_ulong_atomic_xor_func>(dlsym(handle, "shmem_ulong_atomic_xor"));

//   p_shmem_ulong_atomic_fetch_nbi = reinterpret_cast<shmem_ulong_atomic_fetch_nbi_func>(dlsym(handle, "shmem_ulong_atomic_fetch_nbi"));
//   p_shmem_ulong_atomic_compare_swap_nbi = reinterpret_cast<shmem_ulong_atomic_compare_swap_nbi_func>(dlsym(handle, "shmem_ulong_atomic_compare_swap_nbi"));
//   p_shmem_ulong_atomic_swap_nbi = reinterpret_cast<shmem_ulong_atomic_swap_nbi_func>(dlsym(handle, "shmem_ulong_atomic_swap_nbi"));
//   p_shmem_ulong_atomic_fetch_inc_nbi = reinterpret_cast<shmem_ulong_atomic_fetch_inc_nbi_func>(dlsym(handle, "shmem_ulong_atomic_fetch_inc_nbi"));
//   p_shmem_ulong_atomic_fetch_add_nbi = reinterpret_cast<shmem_ulong_atomic_fetch_add_nbi_func>(dlsym(handle, "shmem_ulong_atomic_fetch_add_nbi"));
//   p_shmem_ulong_atomic_fetch_and_nbi = reinterpret_cast<shmem_ulong_atomic_fetch_and_nbi_func>(dlsym(handle, "shmem_ulong_atomic_fetch_and_nbi"));
//   p_shmem_ulong_atomic_fetch_or_nbi = reinterpret_cast<shmem_ulong_atomic_fetch_or_nbi_func>(dlsym(handle, "shmem_ulong_atomic_fetch_or_nbi"));
//   p_shmem_ulong_atomic_fetch_xor_nbi = reinterpret_cast<shmem_ulong_atomic_fetch_xor_nbi_func>(dlsym(handle, "shmem_ulong_atomic_fetch_xor_nbi"));

//   /* Signaling Operations */
//   p_shmem_signal_fetch = reinterpret_cast<shmem_signal_fetch_func>(dlsym(handle, "shmem_signal_fetch"));
//   p_shmem_long_put_signal = reinterpret_cast<shmem_long_put_signal_func>(dlsym(handle, "shmem_long_put_signal"));
//   p_shmem_long_put_signal_nbi = reinterpret_cast<shmem_long_put_signal_nbi_func>(dlsym(handle, "shmem_long_put_signal_nbi"));

//   /* Collective Routines */
//   p_shmem_sync = reinterpret_cast<shmem_sync_func>(dlsym(handle, "shmem_sync"));
//   p_shmem_sync_all = reinterpret_cast<shmem_sync_all_func>(dlsym(handle, "shmem_sync_all"));
//   p_shmem_long_alltoall = reinterpret_cast<shmem_long_alltoall_func>(dlsym(handle, "shmem_long_alltoall"));
//   p_shmem_long_alltoalls = reinterpret_cast<shmem_long_alltoalls_func>(dlsym(handle, "shmem_long_alltoalls"));
//   p_shmem_long_broadcast = reinterpret_cast<shmem_long_broadcast_func>(dlsym(handle, "shmem_long_broadcast"));
//   p_shmem_long_collect = reinterpret_cast<shmem_long_collect_func>(dlsym(handle, "shmem_long_collect"));
//   p_shmem_long_fcollect = reinterpret_cast<shmem_long_fcollect_func>(dlsym(handle, "shmem_long_fcollect"));
//   p_shmem_long_and_reduce = reinterpret_cast<shmem_long_and_reduce_func>(dlsym(handle, "shmem_long_and_reduce"));
//   p_shmem_long_or_reduce = reinterpret_cast<shmem_long_or_reduce_func>(dlsym(handle, "shmem_long_or_reduce"));
//   p_shmem_long_xor_reduce = reinterpret_cast<shmem_long_xor_reduce_func>(dlsym(handle, "shmem_long_xor_reduce"));
//   p_shmem_long_max_reduce = reinterpret_cast<shmem_long_max_reduce_func>(dlsym(handle, "shmem_long_max_reduce"));
//   p_shmem_long_min_reduce = reinterpret_cast<shmem_long_min_reduce_func>(dlsym(handle, "shmem_long_min_reduce"));
//   p_shmem_long_sum_reduce = reinterpret_cast<shmem_long_sum_reduce_func>(dlsym(handle, "shmem_long_sum_reduce"));
//   p_shmem_long_prod_reduce = reinterpret_cast<shmem_long_prod_reduce_func>(dlsym(handle, "shmem_long_prod_reduce"));

//   /* Point-to-Point Synchronization Routines */
//   p_shmem_long_wait_until = reinterpret_cast<shmem_long_wait_until_func>(dlsym(handle, "shmem_long_wait_until"));
//   p_shmem_long_wait_until_all = reinterpret_cast<shmem_long_wait_until_all_func>(dlsym(handle, "shmem_long_wait_until_all"));
//   p_shmem_long_wait_until_any = reinterpret_cast<shmem_long_wait_until_any_func>(dlsym(handle, "shmem_long_wait_until_any"));
//   p_shmem_long_wait_until_some = reinterpret_cast<shmem_long_wait_until_some_func>(dlsym(handle, "shmem_long_wait_until_some"));
//   p_shmem_long_wait_until_all_vector = reinterpret_cast<shmem_long_wait_until_all_vector_func>(dlsym(handle, "shmem_long_wait_until_all_vector"));
//   p_shmem_long_wait_until_any_vector = reinterpret_cast<shmem_long_wait_until_any_vector_func>(dlsym(handle, "shmem_long_wait_until_any_vector"));
//   p_shmem_long_wait_until_some_vector = reinterpret_cast<shmem_long_wait_until_some_vector_func>(dlsym(handle, "shmem_long_wait_until_some_vector"));
//   p_shmem_long_test = reinterpret_cast<shmem_long_test_func>(dlsym(handle, "shmem_long_test"));
//   p_shmem_long_test_all = reinterpret_cast<shmem_long_test_all_func>(dlsym(handle, "shmem_long_test_all"));
//   p_shmem_long_test_any = reinterpret_cast<shmem_long_test_any_func>(dlsym(handle, "shmem_long_test_any"));
//   p_shmem_long_test_some = reinterpret_cast<shmem_long_test_some_func>(dlsym(handle, "shmem_long_test_some"));
//   p_shmem_long_test_all_vector = reinterpret_cast<shmem_long_test_all_vector_func>(dlsym(handle, "shmem_long_test_all_vector"));
//   p_shmem_long_test_any_vector = reinterpret_cast<shmem_long_test_any_vector_func>(dlsym(handle, "shmem_long_test_any_vector"));
//   p_shmem_long_test_some_vector = reinterpret_cast<shmem_long_test_some_vector_func>(dlsym(handle, "shmem_long_test_some_vector"));
//   p_shmem_signal_wait_until = reinterpret_cast<shmem_signal_wait_until_func>(dlsym(handle, "shmem_signal_wait_until"));

//   /* Memory Ordering Routines */
//   p_shmem_quiet = reinterpret_cast<shmem_quiet_func>(dlsym(handle, "shmem_quiet"));
//   p_shmem_fence = reinterpret_cast<shmem_fence_func>(dlsym(handle, "shmem_fence"));

//   /* Distributed Locking Routines */
//   p_shmem_set_lock = reinterpret_cast<shmem_set_lock_func>(dlsym(handle, "shmem_set_lock"));
//   p_shmem_clear_lock = reinterpret_cast<shmem_clear_lock_func>(dlsym(handle, "shmem_clear_lock"));

//   const char *dlsym_error = dlerror();
//   if (dlsym_error) {
//     std::cerr << "Error loading functions: " << dlsym_error << std::endl;
//     dlclose(handle);
//     return false;
//   }

//   return true;
// }