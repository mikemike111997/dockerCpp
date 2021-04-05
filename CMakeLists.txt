cmake_minimum_required(VERSION 3.16.3)
project(DockerCpp VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
    set(BUILD_SHARED_LIBS TRUE)

    # The preprocessor macros WINVER and WIN32_WINNT
    # specify the minimum operating system version your code supports
    # 0x0A00 // Windows 10
    add_compile_options(/WX /W3 -D_WIN32_WINNT=0x0A00 -D_WINVER=0x0A00)

    # Resolve build dir lib lookup
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
else()
    add_compile_options(-g2 -Werror -Wall -Wpedantic -Weffc++)
endif()

find_package(Threads REQUIRED)
find_package(Boost REQUIRED)

include(cmake/add-cppcheck-target.cmake)

add_subdirectory(dbLayer)
add_subdirectory(httpServer)
add_subdirectory(tcpServer)
