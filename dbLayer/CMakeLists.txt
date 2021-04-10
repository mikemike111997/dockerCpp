find_package(OpenSSL REQUIRED)
find_package(PostgreSQL REQUIRED)

add_library(dbLayer SHARED src/dbConnection.cpp inc/dbConnection.h)
target_link_libraries(dbLayer PUBLIC PostgreSQL::PostgreSQL
                              PRIVATE OpenSSL::SSL OpenSSL::Crypto Threads::Threads)
target_include_directories(dbLayer PUBLIC inc PRIVATE ${PostgreSQL_INCLUDE_DIR})

# rpc server
add_executable(dbLayerRPCServer src/dbLayerRPCServer.cpp)
target_link_libraries(dbLayerRPCServer
                            PUBLIC dbLayer
                            PRIVATE dbLayerRPC Boost::boost
                            PRIVATE OpenSSL::SSL OpenSSL::Crypto Threads::Threads
                            PRIVATE gRPC::grpc++ protobuf::libprotobuf Threads::Threads)
target_include_directories(dbLayerRPCServer
                            PUBLIC inc
                            PRIVATE ${PostgreSQL_INCLUDE_DIR})

install(TARGETS dbLayer dbLayerRPCServer)
