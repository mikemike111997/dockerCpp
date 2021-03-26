cmake_minimum_required(VERSION 3.16.3)
project(DockerCpp VERSION 0.1.0)

add_compile_options(-Werror -Wall -Wpedantic -Weffc++)

include(cmake/init-conan.cmake)
conan_cmake_run(CONANFILE conanfile.txt  # or relative build/conanfile.txt
                BASIC_SETUP CMAKE_TARGETS
                BUILD missing)

include(${CMAKE_BINARY_DIR}/conan_paths.cmake)
include(cmake/add-cppcheck-target.cmake)

add_executable(dockerCpp src/main.cpp)
