add_executable(tcpServer src/tcpServer.cpp)
target_link_libraries(tcpServer PRIVATE Boost::boost Threads::Threads)
target_include_directories(tcpServer PUBLIC inc PRIVATE ${Boost_INCLUDE_DIR})

install(TARGETS tcpServer)
