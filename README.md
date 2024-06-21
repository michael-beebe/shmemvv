# SHMEMVV
Validation/Verification test suite for OpenSHMEM (v1.5)

## Requirements
- Implementation of the OpenSHMEM v1.5 specification
- CMake/CTest

## Building/Compiling
```bash
mkdir -p build
cd build
cmake ../
```

## Tests
By default, SHMEMVV will run all the tests, but if you want to only run specific set(s) of tests, you can enable or or multiple of the following flags in your cmake command.

### Library Setup, Exit, and Query Routines
```
-DTEST_SETUP
```
- SHMEM_INIT
- SHMEM_MY_PE
- SHMEM_N_PES
- SHMEM_FINALIZE
- SHMEM_GLOBAL_EXIT
- SHMEM_PE_ACCESSIBLE
- SHMEM_PTR
- SHEM_INFO_GET_VERSION
- SHMEM_INFO_GET_NAME
- START_PES

### Thread Support
```
-DTEST_THREADS
```
- SHMEM_INIT_THREAD
- SHMEM_QUERY_THREAD

### Memory Management Routines
```
-DTEST_MEM
```
- SHMEM_MALLOC
- SHMEM_FREE
- SHMEM_REALLOC
- SHMEM_ALIGN
- SHMEM_MALLOC_WITH_HINTS
- SHMEM_CALLOC

### Team Managment Routines
```
-DTEST_TEAMS
```
- SHMEM_TEAM_MY_PE
- SHMEM_TEAM_N_PES
- SHMEM_TEAM_CONFIG_T
- SHMEM_TEAM_GET_CONFIG
- SHMEM_TEAM_TRANSLATE_PE
- SHMEM_TEAM_SPLIT_STRIDED
- SHMEM_TEAM_SPLIT_2D
- SHMEM_TEAM_DESTROY

### Communication/Context Management Routines
```
-DTEST_COMMS
```
- SHMEM_CTX_CREATE
- SHMEM_TEAM_CREATE_CTX
- SHMEM_CTX_DESTROY
- SHMEM_CTX_GET_TEAM

### Remote Access Routines
```
-DTEST_REMOTE
```
- SHMEM_PUT
- SHMEM_P
- SHMEM_IPUT
- SHMEM_GET
- SHMEM_G
- SHMEM_IGET
- SHMEM_PUT_NBI
- SHMEM_GET_NBI

### Atomic Memory Operations
```
-DTEST_ATOMICS
```
- SHMEM_ATOMIC_FETCH
- SHMEM_ATOMIC_SET
- SHMEM_ATOMIC_COMPARE_SWAP
- SHMEM_ATOMIC_SWAP
- SHMEM_ATOMIC_FETCH_INC
- SHMEM_ATOMIC_INC
- SHMEM_ATOMIC_FETCH_ADD
- SHMEM_ATOMIC_ADD
- SHMEM_ATOMIC_FETCH_AND
- SHMEM_ATOMIC_AND
- SHMEM_ATOMIC_FETCH_OR
- SHMEM_ATOMIC_OR
- SHMEM_ATOMIC_FETCH_XOR
- SHMEM_ATOMIC_XOR
- SHMEM_ATOMIC_FETCH_NBI
- SHMEM_ATOMIC_COMPARE_SWAP_NBI
- SHMEM_ATOMIC_SWAP_NBI
- SHMEM_ATOMIC_FETCH_INC_NBI
- SHMEM_ATOMIC_FETCH_ADD_NBI
- SHMEM_ATOMIC_FETCH_AND_NBI
- SHMEM_ATOMIC_FETCH_OR_NBI
- SHMEM_ATOMIC_FETCH_XOR_NBI

### Signaling Operations
```
-DTEST_SIGNALING
```
- SHMEM_PUT_SIGNAL
- SHMEM_PUT_SIGNAL_NBI
- SHMEM_SIGNAL_FETCH

### Collective Routines
```
-DTEST_COLLECTIVES
```
- SHMEM_BARRIER_ALL
- SHMEM_BARRIER
- SHMEM_SYNC
- SHMEM_SYNC_ALL
- SHMEM_ALLTOALL
- SHMEM_ALLTOALLS
- SHMEM_BROADCAST
- SHMEM_COLLECT
- SHMEM_FCOLLECT
- SHMEM_REDUCTIONS
  - AND, OR, XOR
  - MAX, MIN, SUM, PROD

### Point-Point Synchronization Routines
```
-DTEST_PT2PT_SYNCH
```
- SHMEM_WAIT_UNTIL
- SHMEM_WAIT_UNTIL_ALL
- SHMEM_WAIT_UNTIL_ANY
- SHMEM_WAIT_UNTIL_SOME
- SHMEM_WAIT_UNTIL_ALL_VECTOR
- SHMEM_WAIT_UNTIL_ANY_VECTOR
- SHMEM_WAIT_UNTIL_SOME_VECTOR
- SHMEM_TEST
- SHMEM_TEST_ALL
- SHMEM_TEST_ANY
- SHMEM_TEST_SOME
- SHMEM_TEST_ALL_VECTOR
- SHMEM_TEST_ANY_VECTOR
- SHMEM_TEST_SOME_VECTOR
- SHMEM_SIGNAL_WAIT_UNTIL

### Memory Ordering Routines
```
-DTEST_MEM_ORDERING
```
- SHMEM_FENCE
- SHMEM_QUIET
- Synchronization and Communication Ordering in OpenSHMEM

### Distributed Locking Routines
```
-DTEST_LOCKING
```
- SHMEM_LOCK
- SHMEM_UNLOCK

