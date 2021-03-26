cmake_minimum_required(VERSION 3.17.3)
project(DockerCpp VERSION 0.1.0)

add_compile_options(-Werror -Wall -Wpedantic -Weffc++)

add_executable(dockerCpp main.cpp)
