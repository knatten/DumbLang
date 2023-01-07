Toy project for learning LLVM and some modern C++ stuff. Don't come here for best practices.

![CI](https://github.com/knatten/DumbLang/actions/workflows/ci.yml/badge.svg)


Currently only tested on LLVM 15

```
mkdir build && cd build
conan install ../src/ --profile=<your conan profile>  --build=missing
cmake ../src/
cmake --build
ctest
```

