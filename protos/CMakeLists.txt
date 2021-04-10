# Proto file
get_filename_component(hw_proto "stringreverse.proto" ABSOLUTE)
get_filename_component(hw_proto_path "${hw_proto}" PATH)

# Proto file
get_filename_component(hw_proto "dbLayer.proto" ABSOLUTE)
get_filename_component(hw_proto_path "${hw_proto}" PATH)

# Generated sources
set(hw_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/dbLayer.pb.cc")
set(hw_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/dbLayer.pb.h")
set(hw_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/dbLayer.grpc.pb.cc")
set(hw_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/dbLayer.grpc.pb.h")

add_custom_command(
      OUTPUT "${hw_proto_srcs}" "${hw_proto_hdrs}" "${hw_grpc_srcs}" "${hw_grpc_hdrs}"
      COMMAND protobuf::protoc
      ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${hw_proto_path}"
        --plugin=protoc-gen-grpc="$<TARGET_FILE:gRPC::grpc_cpp_plugin>"
        "${hw_proto}"
      DEPENDS "${hw_proto}")

add_library(dbLayerRPC OBJECT ${hw_proto_srcs} ${hw_grpc_srcs} ${hw_proto_hdrs} ${hw_grpc_hdrs})
target_include_directories(dbLayerRPC PUBLIC ${CMAKE_CURRENT_BINARY_DIR}
                                      PUBLIC ${Protobuf_INCLUDE_DIRS})

if(MSVC)
  target_compile_options(dbLayerRPC BEFORE PRIVATE /W0)
else()
  # suppress grpc and protobuf warnings
  target_compile_options(dbLayerRPC BEFORE PRIVATE -Wno-zero-length-array -Wno-non-virtual-dtor)
endif()
