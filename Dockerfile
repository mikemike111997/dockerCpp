# Get the base Ubuntu image from Docker Hub
FROM ubuntu:latest

# Update apps on the base image
RUN apt-get -y update && apt-get install -y cmake ninja-build gdb valgrind cppcheck

# Install the Clang compiler
RUN apt-get -y install clang

# Install git
RUN apt-get -y install git

# Install pip
RUN apt-get -y install python3-pip

# Install conan
RUN pip3 install conan

# Use clang as default compiler
RUN export CXX=/usr/bin/clang++

CMD ["bash"]
