find_package(OpenSSL REQUIRED)
find_package(PostgreSQL REQUIRED)

add_library(dbLayer SHARED src/dbConnection.cpp inc/dbConnection.h)
target_link_libraries(dbLayer PUBLIC PostgreSQL::PostgreSQL PRIVATE OpenSSL::SSL OpenSSL::Crypto Threads::Threads)
target_include_directories(dbLayer PUBLIC inc PRIVATE ${PostgreSQL_INCLUDE_DIR})

install(TARGETS dbLayer)
