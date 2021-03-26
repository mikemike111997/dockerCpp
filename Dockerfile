# Get the base Ubuntu image from Docker Hub
FROM ubuntu:latest

# Update apps on the base image
RUN apt-get -y update && apt-get install -y cmake ninja-build gdb valgrind cppcheck

# Install the Clang compiler
RUN apt-get -y install clang

# Install git
RUN apt-get -y install git

# Use clang as default compiler
RUN export CXX=/usr/bin/clang++

CMD ["bash"]