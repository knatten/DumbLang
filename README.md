Toy project for learning LLVM and some modern C++ stuff. Don't come here for best practices.

Currently only tested on LLVM 15

```
mkdir build && cd build
conan install ../src/ --profile=<your conan profile>  --build=missing
cmake ../src/
cmake --build
./bin/test #TODO use ctest
```

