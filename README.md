Toy project for learning LLVM and some modern C++ stuff. Don't come here for best practices.

This only works on my machine, due to the custom clang that I didn't bother generalising

```
mkdir build && cd build
conan install ../src/ --profile=../src/conan-profiles/release  --build=missing
cmake -GNinja -DCMAKE_C_COMPILER=/opt/homebrew/Cellar/llvm/15.0.6/bin/clang -DCMAKE_CXX_COMPILER=/opt/homebrew/Cellar/llvm/15.0.6/bin/clang++ ../src/
ninja
./bin/test
```
