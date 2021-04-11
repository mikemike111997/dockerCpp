#pragma once

/**
 * @file dbLayerRPCClient.h
 * @author Mykhailo Lohvynenko (mikemike111997@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2021-04-11
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifdef _MSC_VER
    #pragma warning( push )
    #pragma warning(disable: 4251) // disable warning 4345
    #pragma warning(disable: 4244) // enable warning 4345 back
    #pragma warning(disable: 4996) // enable warning 4345 back
#endif

#include "dbLayer.grpc.pb.h"
#include <grpc++/channel.h>

#ifdef _MSC_VER
    #pragma warning( pop )
#endif

#include <memory>
#include <vector>
#include <string>


class DBClinetRPC
{
public:
    DBClinetRPC(std::shared_ptr<grpc::Channel> channel) :
        stub_(dbLayer::dbHandler::NewStub(channel)) {}
    
    dbLayer::Users getAllUsers();

    static std::unique_ptr<DBClinetRPC> Create(std::string_view serverAddr);

private:
    std::unique_ptr<dbLayer::dbHandler::Stub>  stub_;
};
