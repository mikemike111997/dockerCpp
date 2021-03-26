# dockerCpp
dockerCpp is a simple playground to learn cmake, dockern and modern C++ features.

# CMake cache generation and build
Clone project and create a build directory:
```bash
git clone https://github.com/mikemike111997/dockerCpp.git
cd dockerCpp && cmake --type=debug -G Ninja .. && ninja
```

Use a desired build system (I prefer Ninja, you can omit -G Ninja flag to use default one):
```bash
cd dockerCpp && cmake --type=debug -G Ninja .. && ninja
```

To build a release build change build type:
```bash
cd dockerCpp && cmake --type=release -G Ninja .. && ninja
```

To build using Unix Makefiles:
```bash
cd dockerCpp && cmake --type=release .. && make
```
