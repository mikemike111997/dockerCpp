cmake_minimum_required(VERSION 3.16.3)
project(DockerCpp VERSION 0.1.0)

add_compile_options(-Werror -Wall -Wpedantic -Weffc++)

add_executable(dockerCpp src/main.cpp)
