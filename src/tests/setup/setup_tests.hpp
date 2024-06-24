#ifndef SETUP_TESTS_HPP
#define SETUP_TESTS_HPP

#include <shmem.h>
#include <iostream>

/* Function declarations for setup tests */
bool test_shmem_init(void);
bool test_shmem_finalize(void);
bool test_shmem_my_pe(void);
bool test_shmem_n_pes(void);
bool test_shmem_pe_accessible(void);
bool test_shmem_ptr(void);
bool test_shmem_info_get_version(void);
bool test_shmem_info_get_name(void);
bool test_start_pes(void);
bool test_shmem_global_exit(void);

#endif /* SETUP_TESTS_HPP */
