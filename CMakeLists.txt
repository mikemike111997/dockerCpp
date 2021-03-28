cmake_minimum_required(VERSION 3.16.3)
project(DockerCpp VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(/WX /W3 -D_WIN32_WINNT=0x0501)
else()
    add_compile_options(-g2 -Werror -Wall -Wpedantic -Weffc++)
endif()


include(cmake/init-conan.cmake)
conan_cmake_run(CONANFILE conanfile.txt  # or relative build/conanfile.txt
                BASIC_SETUP CMAKE_TARGETS
                BUILD missing)

include(${CMAKE_BINARY_DIR}/conan_paths.cmake)
include(cmake/add-cppcheck-target.cmake)

add_subdirectory(tcpServer)