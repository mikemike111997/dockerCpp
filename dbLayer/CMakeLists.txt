add_library(dbLayer SHARED src/dbConnection.cpp inc/dbConnection.h)

find_package(OpenSSL REQUIRED)
find_package(PostgreSQL REQUIRED)

target_link_libraries(dbLayer PostgreSQL::PostgreSQL PUBLIC OpenSSL::SSL OpenSSL::Crypto Threads::Threads)
target_include_directories(dbLayer PUBLIC inc PUBLIC ${PostgreSQL_INCLUDE_DIR})

install(TARGETS dbLayer)
