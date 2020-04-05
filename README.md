![C/C++ CI](https://github.com/malikkirchner/connection-pool/workflows/C/C++%20CI/badge.svg?branch=master)

# Connection Pool

A generic connection pool for C++.

## Build and test

From the repository root run following commands to build and test this library.
```bash
# make build directory
mkdir build
cd build
# configure build system
cmake .. -DCMAKE_BUILD_TYPE=Debug|Release|...
# build
cmake --build .
# test
cmake --build . --target test
```

## Usage

Take a look at `test/unit_test.cpp` for an example.
