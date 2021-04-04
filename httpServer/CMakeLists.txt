add_executable(httpServer src/httpServer.cpp src/dbConnection.cpp inc/dbConnection.h)
target_link_libraries(httpServer CONAN_PKG::boost CONAN_PKG::libpq)
target_include_directories(httpServer PUBLIC inc PUBLIC ${CONAN_BOOST_ROOT}/include ${CONAN_LIBPQ_ROOT}/include)

install(TARGETS httpServer)
