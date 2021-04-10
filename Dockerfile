# Get the base Ubuntu image from Docker Hub
FROM ubuntu:latest

# Update apps on the base image
RUN apt-get -y update && apt-get install -y cmake ninja-build gdb valgrind cppcheck

# Install the Clang compiler
RUN apt-get -y install clang

# Install git
RUN apt-get -y install git

# Install python3
RUN apt-get -y install python3

# Install pip
RUN apt-get -y install python3-pip

# Install conan
RUN pip3 install conan

RUN apt-get -y install curl zip unzip tar

RUN apt-get -y install pkg-config

# Use clang as default compiler
ENV CXX=/usr/bin/clang++

WORKDIR /usr/src
# Install vcpkg
RUN git clone https://github.com/microsoft/vcpkg
RUN cd vcpkg && ./bootstrap-vcpkg.sh
ENV PATH="/usr/src/vcpkg:${PATH}"

RUN vcpkg install boost
RUN vcpkg install libpq
RUN vcpkg install grpc
RUN vcpkg install protobuf
RUN vcpkg integrate install

ADD . /usr/src/dockerCpp/

# make sure build folder is created and it's clean
RUN rm -rf /usr/src/dockerCpp/build
RUN mkdir -p /usr/src/dockerCpp/build
WORKDIR /usr/src/dockerCpp/build

# Generate CMake cache
RUN cmake -G Ninja -DCMAKE_INSTALL_PREFIX=/usr/local -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_TOOLCHAIN_FILE=/usr/src/vcpkg/scripts/buildsystems/vcpkg.cmake ..
ENV LD_LIBRARY_PATH="/usr/local/lib:${LD_LIBRARY_PATH}"

# build and install targets
RUN ninja install

EXPOSE 8080

WORKDIR /usr/local
CMD ["httpServer", "0.0.0.0", "8080"]
