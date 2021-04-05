add_executable(httpServer src/httpServer.cpp)
target_link_libraries(httpServer dbLayer Boost::boost Threads::Threads)
target_include_directories(httpServer PUBLIC inc
    PRIVATE ../dbLayer/inc
    PUBLIC ${Boost_INCLUDE_DIR})

install(TARGETS httpServer)
