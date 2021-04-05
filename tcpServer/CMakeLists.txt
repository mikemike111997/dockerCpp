add_executable(tcpServer src/tcpServer.cpp)
target_link_libraries(tcpServer Boost::boost Threads::Threads)
target_include_directories(tcpServer PUBLIC inc PUBLIC ${Boost_INCLUDE_DIR})

install(TARGETS tcpServer)
