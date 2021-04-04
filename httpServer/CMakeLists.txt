add_executable(httpServer src/httpServer.cpp)
target_link_libraries(httpServer CONAN_PKG::boost)
target_include_directories(httpServer PUBLIC inc PUBLIC ${CONAN_BOOST_ROOT}/include)

install(TARGETS httpServer)
