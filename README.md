# Astra ★

My small project to implement games and graphics applications with GLFW, GLEW(OpenGL) in C++

In theory, cross-platform. Compilation is done with cmake

## Build
Requirements:
-  CMake + Make
-  GCC (MinGW on Windows, can't get it right with LLVM/Clang...)

### Linux
```
cmake . -B ./build
make -C ./build
```
### Windows
```
cmake . -B ./build -G "MinGW Makefiles"
make -C ./build
```

## Dependencies
-  GLFW
-  GLEW
