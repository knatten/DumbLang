Toy project for learning LLVM and some modern C++ stuff. Don't come here for best practices.

![CI](https://github.com/knatten/DumbLang/actions/workflows/ci.yml/badge.svg)
[![Contributor Covenant](https://img.shields.io/badge/Contributor%20Covenant-2.1-4baaaa.svg)](code_of_conduct.md)



Requirements:
- Currently only tested on LLVM 15, on macOS and Linux.
- Python
- `pip install -r ci/requirements.txt` (preferably in a virtualenv)
- If you want to use the Docker script, you also need Docker (on macOS, also coreutils, e.g. `brew install coreutils`)

### Setup and build

```
mkdir build && cd build
conan install ../src/ -pr=<your conan profile> --build=missing -of=.
cmake ../src/
cmake --build -DCMAKE_BUILD_TYPE=<See below>
ctest
```

Make sure you use a `CMAKE_BUILD_TYPE` corresponding to the build type in the Conan profile you specified. For more
advanced use cases, see https://docs.conan.io/en/latest/reference/conanfile/tools/cmake/cmakedeps.html

### CI
```
./ci/build-and-test.sh <Debug|Release|All>
```

To run in Docker, instead do
```
./ci/docker_run.sh ./ci/build-and-test.sh <Debug|Release|All>
```