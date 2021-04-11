#include "dbLayerRPCClient.h"
#include <grpcpp/create_channel.h>
using dbLayer::dbHandler;


dbLayer::Users DBClinetRPC::getAllUsers()
{
    grpc::ClientContext  context;
    google::protobuf::Empty request;
    dbLayer::Users response;
    stub_->getAllUsers(&context, request, &response);

    return response;
}

std::unique_ptr<DBClinetRPC> DBClinetRPC::Create(std::string_view serverAddr)
{
    return std::make_unique<DBClinetRPC>(
        grpc::CreateChannel(serverAddr.data(),
                            grpc::InsecureChannelCredentials()));
}
