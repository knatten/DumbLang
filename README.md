Toy project for learning LLVM and some modern C++ stuff. Don't come here for best practices.

![CI](https://github.com/knatten/DumbLang/actions/workflows/ci.yml/badge.svg)


Requirements:
- Currently only tested on LLVM 15, on macOS and Linux.
- Conan. E.g. `pip install conan`. Profiles exist for Linux/x86_64 and Mac/armv8.

### Setup and build

```
mkdir build && cd build
conan install ../src/ --profile=<your conan profile>  --build=missing
cmake ../src/
cmake --build -DCMAKE_BUILD_TYPE=<See below>
ctest
```

Make sure you use a `CMAKE_BUILD_TYPE` corresponding to the build type in the Conan profile you specified. For more
advanced use cases, see https://docs.conan.io/en/latest/reference/conanfile/tools/cmake/cmakedeps.html

### CI
```
./ci/build-and-test.sh
```