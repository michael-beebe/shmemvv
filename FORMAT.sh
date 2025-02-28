#!/bin/bash

# Find all .c, .cpp, and .h files in src directory and subdirectories
# and run clang-format on them in-place
find src -type f \( -name "*.c" -o -name "*.cpp" -o -name "*.h" \) -exec clang-format -i {} +

echo "Formatting complete!"

