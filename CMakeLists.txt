cmake_minimum_required(VERSION 3.16.3)
project(DockerCpp VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
add_compile_options(-g2 -Werror -Wall -Wpedantic -Weffc++)

include(cmake/init-conan.cmake)
conan_cmake_run(CONANFILE conanfile.txt  # or relative build/conanfile.txt
                BASIC_SETUP CMAKE_TARGETS
                BUILD missing)

include(${CMAKE_BINARY_DIR}/conan_paths.cmake)
include(cmake/add-cppcheck-target.cmake)

add_executable(dockerCpp src/main.cpp)
target_link_libraries(dockerCpp CONAN_PKG::boost)
target_include_directories(dockerCpp PUBLIC inc)

install(TARGETS dockerCpp)
