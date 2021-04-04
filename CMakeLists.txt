cmake_minimum_required(VERSION 3.16.3)
project(DockerCpp VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

if(MSVC)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
    set(BUILD_SHARED_LIBS TRUE)

    # The preprocessor macros WINVER and WIN32_WINNT
    # specify the minimum operating system version your code supports
    # 0x0A00 // Windows 10
    add_compile_options(/WX /W3 -D_WIN32_WINNT=0x0A00 -D_WINVER=0x0A00)
else()
    add_compile_options(-g2 -Werror -Wall -Wpedantic -Weffc++)
endif()




include(cmake/init-conan.cmake)
conan_cmake_run(CONANFILE conanfile.txt  # or relative build/conanfile.txt
                BASIC_SETUP CMAKE_TARGETS
                BUILD missing)

include(${CMAKE_BINARY_DIR}/conan_paths.cmake)
include(cmake/add-cppcheck-target.cmake)

add_subdirectory(dbLayer)
add_subdirectory(httpServer)
add_subdirectory(tcpServer)
