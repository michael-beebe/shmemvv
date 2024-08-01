/* Agnostic tests */
#include "../tests/setup/setup_tests.h"
#include "../tests/threads/threads_tests.h"
#include "../tests/mem/mem_tests.h"
#include "../tests/comms/comms_tests.h"
#include "../tests/teams/teams_tests.h"
#include "../tests/mem_ordering/mem_ordering_tests.h"
#include "../tests/locking/locking_tests.h"

/* C/C++ tests */
#include "../tests/remote/cxx_remote_tests.h"
#include "../tests/atomics/cxx_atomics_tests.h"
#include "../tests/signaling/cxx_signaling_tests.h"
#include "../tests/collectives/cxx_collectives_tests.h"
#include "../tests/pt2pt/cxx_pt2pt_tests.h"

/* C11 tests */
#include "../tests/remote/c11_remote_tests.h"
#include "../tests/atomics/c11_atomics_tests.h"
