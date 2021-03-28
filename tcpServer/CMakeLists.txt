add_executable(tcpServer src/tcpServer.cpp)
target_link_libraries(tcpServer CONAN_PKG::boost)
target_include_directories(tcpServer PUBLIC inc)

install(TARGETS tcpServer)
