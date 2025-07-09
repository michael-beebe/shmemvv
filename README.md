# SHMEMVV - OpenSHMEM Verification & Validation Suite

A comprehensive testing suite for the [OpenSHMEM v1.5 specification](http://www.openshmem.org/site/sites/default/site_files/OpenSHMEM-1.5.pdf). This suite provides extensive coverage of the OpenSHMEM API across both C and C11 language bindings.

## Overview

SHMEMVV tests the correctness and conformance of OpenSHMEM implementations by systematically exercising all aspects of the v1.5 specification, including:

- Memory management
- Communication contexts
- Remote memory access
- Atomic memory operations
- Team-based operations
- Signaling operations
- Point-to-point synchronization
- Collectives
- Distributed locking
- Thread safety

Tests are implemented in standard C for all categories, with additional C11 variants available for atomic operations, collective operations, point-to-point synchronization, remote memory access, and signaling operations.

## Requirements

- An implementation of OpenSHMEM v1.5
- CMake 3.10 or greater
- C compiler supporting both standard C and C11 standards

## Environment Setup

Before building the test suite, ensure your OpenSHMEM environment is properly configured:

1. **Setting up OpenSHMEM compiler wrappers**:
   - The OpenSHMEM implementation must provide the `oshcc` compiler wrapper
   - This wrapper must be in your PATH
   - Verify with: `which oshcc`

2. **Setting up the OpenSHMEM launcher**:
   - The OpenSHMEM implementation must provide the `oshrun` launcher
   - This launcher must be in your PATH
   - Verify with: `which oshrun`
   - Some implementations may use a different launcher name. In that case, you'll need to specify it with the `--launcher` option when running the tests

3. **OpenSHMEM runtime configuration**:
   - Some OpenSHMEM implementations require additional environment variables
   - Consult your OpenSHMEM implementation's documentation for specific requirements
   - Common variables include:
     - `SHMEM_SYMMETRIC_SIZE`: Size of the symmetric heap
     - `SHMEM_DEBUG`: Enable debugging output

## Building the Test Suite

1. Clone the repository:
   ```bash
   git clone https://github.com/michael-beebe/shmemvv.git
   cd shmemvv
   ```

2. Create a build directory:
   ```bash
   mkdir build
   cd build
   ```

3. Configure with CMake:
   ```bash
   # Basic configuration using the oshcc in your PATH
   cmake ..
   
   # Or explicitly specify the compiler path
   cmake \
    -DCMAKE_C_COMPILER=$(which oshcc) \
    ..
   
   # If your OpenSHMEM implementation's compiler is installed in a non-standard location:
   cmake \
    -DCMAKE_C_COMPILER=/path/to/custom/installation/bin/oshcc \
    ..
   ```

4. Build the test suite:
   ```bash
   make
   ```

### CMake Build Options

You can customize the build using various CMake options:

- `-DDISABLE_ALL_C_TESTS=ON`: Skip building all standard C tests
- `-DDISABLE_ALL_C11_TESTS=ON`: Skip building all C11 tests 
- `-DDISABLE_C_ATOMICS=ON`: Skip building standard C atomic tests
- `-DDISABLE_C11_ATOMICS=ON`: Skip building C11 atomic tests
- `-DCMAKE_INSTALL_PREFIX=/path/to/install`: Specify installation directory

Example with multiple options:
```bash
cmake \
  -DCMAKE_C_COMPILER=$(which oshcc) \
  -DDISABLE_ALL_C11_TESTS=ON        \
  ..
```

For a complete list of build options, refer to the `CMakeLists.txt` file.

## Running Tests

The test suite comes with a versatile test runner script that allows you to selectively run different categories of tests:

```bash
./shmemvv.sh --enable_c --enable_c11  # Run both standard C and C11 tests
```

### Launcher Configuration

By default, the test script uses `oshrun` as the launcher. If your OpenSHMEM implementation uses a different launcher or if the launcher is in a non-standard location:

```bash
# Use a different launcher
./shmemvv.sh --enable_c --launcher mpirun

# Specify the full path to the launcher
./shmemvv.sh --enable_c --launcher /path/to/custom/oshrun 

# Add custom arguments to the launcher
./shmemvv.sh --enable_c --launcher mpirun --launcher_args "--oversubscribe --map-by node"
```

### Test Categories

You can run specific test categories using the following options:

```bash
./shmemvv.sh --enable_c --test_atomics            # Run only atomic tests for standard C
./shmemvv.sh --enable_c11 --test_collectives      # Run only collective tests for C11
./shmemvv.sh --enable_c --test_setup               # Run setup tests (C11 not available for setup)
./shmemvv.sh --enable_c --enable_c11 --test_remote # Run remote memory access tests for both C and C11
```

### Language Selection

- `--enable_c`: Enable standard C tests (available for all test categories)
- `--enable_c11`: Enable C11 tests (available for: atomics, collectives, pt2pt_sync, rma, signaling)

You must enable at least one of these options.

### Process Count Control

You can override the default number of PEs (Processing Elements) for all tests:

```bash
./shmemvv.sh --enable_c --enable_c11 --np 4  # Run all tests with 4 PEs
```

### Excluding Categories

You can exclude specific test categories:

```bash
./shmemvv.sh --enable_c --enable_c11 --exclude_pt2pt_synch  # Run everything except point-to-point sync tests
```

### Full Options List

The complete list of available options for the `shmemvv.sh` script:

```
OpenSHMEM Verification and Validation Test Suite

Usage: ./shmemvv.sh [options]

Options:
  --test_setup            Run setup tests
  --test_threads          Run thread support tests
  --test_mem              Run memory management tests
  --test_teams            Run team management tests
  --test_ctx              Run communication/context management tests
  --test_remote           Run remote memory access tests
  --test_atomics          Run atomic memory operations tests
  --test_signaling        Run signaling operations tests
  --test_collectives      Run collective operations tests
  --test_pt2pt_synch      Run point-to-point synchronization tests
  --test_locking          Run distributed locking tests
  --test_all              (default) Run all tests

  --exclude_setup         Exclude setup tests
  --exclude_threads       Exclude thread support tests
  --exclude_mem           Exclude memory management tests
  --exclude_teams         Exclude team management tests
  --exclude_ctx           Exclude communication/context management tests
  --exclude_remote        Exclude remote memory access tests
  --exclude_atomics       Exclude atomic memory operations tests
  --exclude_signaling     Exclude signaling operations tests
  --exclude_collectives   Exclude collective operations tests
  --exclude_pt2pt_synch   Exclude point-to-point synchronization tests
  --exclude_locking       Exclude distributed locking tests

  --np <N>                (default=varies by test) Override default PE count for all tests
  --launcher <cmd>        (default=/root/sw/linuxkit-aarch64/sos_1.5.2/bin/oshrun) Path to oshrun launcher
  --launcher_args <args>  Add custom arguments to launcher
  --enable_c11            Enable C11 tests (available for: atomics, collectives, pt2pt_sync, rma, signaling)
  --enable_c              Enable C/C++ tests
  --verbose               Enable verbose output
  --no-color              Disable colored output
  --help                  Display this help message

Note: You must enable at least one of --enable_c or --enable_c11
```

## Test Results

Each test reports whether it passed or failed. The script provides a summary at the end of the run. Detailed logs are saved in the `logs/` directory.

## Advanced Usage

### Custom Launcher

You can specify a custom launcher for running MPI processes:

```bash
./shmemvv.sh --enable_c --launcher mpirun --launcher_args "--oversubscribe"
```

### Running Multiple Test Categories

You can combine multiple test categories in a single run:

```bash
./shmemvv.sh --enable_c --test_atomics --test_collectives --test_remote
```

## Interpreting Test Output

The test output provides information about each test run, including:

- Test name and category
- Number of PEs used
- Success or failure status
- Error messages for failed tests

Detailed logs for each test are stored in the logs directory for further analysis.

## Troubleshooting

### Common Issues

1. **Tests taking too long**: Some tests, especially point-to-point synchronization tests, may take longer to run. Use the `--exclude_pt2pt_synch` option if needed.

2. **Build failures**: Ensure your compiler supports C11 features if building C11 tests.

3. **Launcher not found**: Make sure the OpenSHMEM launcher (`oshrun` by default) is in your PATH. Use `which oshrun` to verify, and consult your OpenSHMEM implementation's documentation for launcher setup instructions.

4. **Compilation errors**: Verify that you're using the OpenSHMEM wrapper compiler (`oshcc`) rather than standard compilers. Check with `which oshcc` to ensure it's properly installed and in your PATH.

5. **"Command not found" errors**: If you encounter issues with missing commands, ensure your OpenSHMEM implementation is properly installed and its environment is sourced in your current shell.

## Contributing

Contributions to improve the test suite are welcome. Please submit pull requests with test improvements, bug fixes, or new tests for untested features.

## License
See LICENSE
