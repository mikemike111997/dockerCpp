add_library(dbLayer SHARED src/dbConnection.cpp inc/dbConnection.h)
target_link_libraries(dbLayer PUBLIC CONAN_PKG::libpq)
target_include_directories(dbLayer PUBLIC inc PUBLIC ${CONAN_LIBPQ_ROOT}/include)

install(TARGETS dbLayer)
