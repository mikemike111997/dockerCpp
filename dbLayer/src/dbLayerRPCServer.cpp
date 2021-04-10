/**
 * @file dbLayerRPCServer.cpp
 * @author Mykhailo Lohvynenko (mikemike111997@gmail.com)
 * @brief A simple gRPC server implementation
 * @version 0.1
 * @date 2021-04-06
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#ifdef _MSC_VER
#pragma warning(disable: 4251) // disable warning 4345
#pragma warning(disable: 4244) // enable warning 4345 back
#pragma warning(disable: 4996) // enable warning 4345 back
#endif

#include "dbLayer.pb.h"
#include "dbLayer.grpc.pb.h"
#include "dbConnection.h"

#include <iostream>
#include <boost/lexical_cast.hpp>
#include <grpc/grpc.h>
#include <grpcpp/server_builder.h>


/**
 * @brief Implement service code for a gRPC server
 * 
 */
class dbLayerServer final : public dbLayer::dbHandler::Service
{
public:
    /**
     * @brief Get the All Users from DB.
     * 
     * @param context grpc obj to control transmition
     * @param request a void param
     * @param response client gets this data after RPC call
     * @return ::grpc::Status 
     */
    virtual ::grpc::Status getAllUsers(::grpc::ServerContext* context,
                                      const ::google::protobuf::Empty* request,
                                      ::dbLayer::Users* response) override
    {
        // (TODO): read DB config from env variables
        NSDBConnection::PGConnection conn("34.65.88.18",
             "5432", "postgres", "postgres", "iDGJ6E7JlkJJ3u5v");
        auto res = conn.executeSQL("select * from users;");
        if (PQresultStatus(res) != PGRES_TUPLES_OK)
        {
            return ::grpc::Status::CANCELLED;
        }
        else
        {
            auto* users = response->mutable_users();
            users->Reserve(PQntuples(res));
            for (int i = 0; i < PQntuples(res); ++i)
            {
                ::dbLayer::User user;
                user.set_id(boost::lexical_cast<int32_t>(PQgetvalue(res, 0, 0)));
                user.set_firstname(PQgetvalue(res, 0, 1));
                user.set_lastname(PQgetvalue(res, 0, 2));
                
                users->Add(std::move(user));
            }
            return ::grpc::Status::OK;
        }
    }
};

/**
 * @brief starts a gRPC server
 * 
 * @param serverAddress 
 */
static void startServer(std::string serverAddress)
{
    dbLayerServer service;

    ::grpc::ServerBuilder builder;
    
    // Listen on the given address without any authentication mechanism
    builder.AddListeningPort(serverAddress, ::grpc::InsecureServerCredentials());
    
    // Register "service" as the instance through which
    // communication with client takes place
    builder.RegisterService(&service);

    // Assembling the server
    std::unique_ptr<::grpc::Server> server(builder.BuildAndStart());
    std::cout << "Server listening on port: " << serverAddress << std::endl;

    server->Wait();
}

int main(int argC, char* argV[])
{
    try
    {
        if(argC != 3)
        {
            std::cerr << "Usage: " << argV[0] << " <address> <port>\n";
            std::cerr << "  For IPv4, try:\n";
            std::cerr << "    receiver 0.0.0.0 80\n";
            std::cerr << "  For IPv6, try:\n";
            std::cerr << "    receiver 0::0 80\n";
            return EXIT_FAILURE;
        }

        std::string serverAddr = argV[1];
        serverAddr.append(":").append(argV[2]);

        startServer(std::move(serverAddr));
    }
    catch(const std::exception& ex)
    {
        std::cout << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
    catch(...)
    {
        std::cout << "Unknown exception caught" << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
