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
cmake --build
ctest
```

### CI
```
./ci/build-and-test.sh
```