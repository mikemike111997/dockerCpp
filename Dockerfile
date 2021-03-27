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
ENV CXX=/usr/bin/clang++

# Install project from git
WORKDIR /usr/src
RUN git clone https://github.com/mikemike111997/dockerCpp.git && \
    cd dockerCpp && mkdir build && cd build && \
    cmake -G Ninja -DCMAKE_BUILD_TYPE=Release .. && ninja install

# Run dockerCpp
CMD ["dockerCpp"]
