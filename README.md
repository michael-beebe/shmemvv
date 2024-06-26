# SHMEMVV
Validation/Verification test suite for OpenSHMEM (v1.5)

![Logo](assets/DALL·E%202024-06-26%2016.04.50%20-%20Create%20a%20logo%20for%20an%20OpenSHMEM%20verification_validation%20test%20suite%20called%20SHMEMVV.%20The%20logo%20should%20incorporate%20the%20same%20colors%20as%20the%20OpenSHMEM%20logo,%20w.webp)

## Requirements
- Implementation of the OpenSHMEM v1.5 specification
- CMake/CTest

## Building
```bash
mkdir -p build
cd build
cmake \
  -DCMAKE_INCLUDE_PATH=</path/to/openshmem/include/dir>  \
  -DCMAKE_LIBRARY_PATH=</path/to/openshmem/lib/dir>      \
  -DCMAKE_C_LINKER_FLAGS="<linker flags, such as -lsma>" \
  ../
```

## Tests
By default, SHMEMVV will run all the tests, but if you want to only run a specific set or sets of tests, you can use one of these runtime flags. For example, this command will run the tests for the communication/context management routines.
```
oshrun -np 2 shmemvv --test_comms
```

Since all the tests need the items within the setup routines, those will be run no matter, regardless of which tests you select.

```bash
Usage: shmemvv [options]
Options:
  --test_setup         Run setup tests
  --test_threads       Run thread support tests
  --test_mem           Run memory management tests
  --test_teams         Run team management tests
  --test_comms         Run communication management tests
  --test_remote        Run remote memory access tests
  --test_atomics       Run atomic memory operations tests
  --test_signaling     Run signaling operations tests
  --test_collectives   Run collective operations tests
  --test_pt2pt_synch   Run point-to-point synchronization tests
  --test_mem_ordering  Run memory ordering tests
  --test_locking       Run distributed locking tests
  --all                (default) Run all tests
  --help               Display help message
```

#### Library Setup, Exit, and Query Routines
```
--test_setup
```
Will ONLY test the following routines:
- shmem_init()
- shmem_my_pe()
- shmem_n_pes()
- shmem_pe_accessible()
- shmem_info_get_version()
- shmem_info_get_name()
- shmem_finalize()
- shmem_global_exit()

#### Thread Support
```
--test_threads
```
Will test the following routines:
- shmem_init_thread()
- shmem_query_thread()

#### Memory Management Routines
```
--test_mem
```
Will test the following routines:
- shmem_ptr()
- shmem_malloc()
- shmem_free()
- shmem_realloc()
- shmem_align()
- shmem_malloc_with_hints()
- shmem_calloc()

#### Team Managment Routines
```
--test_teams
```
Will test the following routines:
- shmem_team_my_pe()
- shmem_team_n_pes()
- shmem_team_config_t()
- shmem_team_get_config()
- shmem_team_translate_pe()
- shmem_team_split_strided()
- shmem_team_split_2d()
- shmem_team_destroy()

#### Communication/Context Management Routines
```
--test_comms
```
Will test the following routines:
- shmem_ctx_create()
- shmem_team_create_ctx()
- shmem_ctx_destroy()
- shmem_ctx_get_team()

#### Remote Access Routines
```
--test_remote
```
Will test the following routines:
- shmem_put()
- shmem_p()
- shmem_iput()
- shmem_get()
- shmem_g()
- shmem_iget()
- shmem_put_nbi()
- shmem_get_nbi()

#### Atomic Memory Operations
```
--test_atomics
```
Will test the following routines:
- shmem_atomic_fetch()
- shmem_atomic_set()
- shmem_atomic_compare_swap()
- shmem_atomic_swap
- shmem_atomic_fetch_inc()
- shmem_atomic_inc()
- shmem_atomic_fetch_add()
- shmem_atomic_add()
- shmem_atomic_fetch_and()
- shmem_atomic_and()
- shmem_atomic_fetch_or()
- shmem_atomic_or()
- shmem_atomic_fetch_xor()
- shmem_atomic_xor()
- shmem_atomic_fetch_nbi()
- shmem_atomic_compare_swap_nbi()
- shmem_atomic_swap_nbi()
- shmem_atomic_fetch_inc_nbi()
- shmem_atomic_fetch_add_nbi()
- shmem_atomic_fetch_and_nbi()
- shmem_atomic_fetch_or_nbi()
- shmem_atomic_fetch_xor_nbi()

#### Signaling Operations
```
--test_signaling
```
Will test the following routines:
- shmem_put_signal()
- shmem_put_signal_nbi()
- shmem_signal_fetch()

#### Collective Routines
```
--test_collectives
```
Will test the following routines:
-  shmem_barrier_all()
- shmem_barrier()
- shmem_sync()
- shmem_sync_all()
- shmem_alltoall()
- shmem_alltoalls()
- shmem_broadcast()
- shmem_collect()
- shmem_fcollect()
- shmem_reductions
    - and, or, xor
    - max, min, sum, prod

#### Point-Point Synchronization Routines
```
--test_pt2pt_synch
```
Will test the following routines:
- shmem_wait_until()
- shmem_wait_until_all()
- shmem_wait_until_any()
- shmem_wait_until_some()
- shmem_wait_until_all_vector()
- shmem_wait_until_any_vector()
- shmem_wait_until_some_vector()
- shmem_test()
- shmem_test_all()
- shmem_test_any()
- shmem_test_some()
- shmem_test_all_vector()
- shmem_test_any_vector()
- shmem_test_some_vector()
- shmem_signal_wait_until()

#### Memory Ordering Routines
```
-test_mem_ordering
```
Will test the following routines:
- shmem_fence()
- shmem_quiet()

#### Distributed Locking Routines
```
-test_locking
```
Will test the following routines:
- shmem_lock()
- shmem_unlock()

#### All tests
```
--test_all
```
Will run all the tests


