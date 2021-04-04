/**
 * @file httpServer.cpp
 * @author Mykhailo Lohvyneno (mikemike111997@gmail.com)
 * @brief Implement a simple HTTP server using boost beast.
          I've referenced boost beast tutorial: 
          https://www.boost.org/doc/libs/develop/libs/beast/example/http/server/small/http_server_small.cpp
 * @version 0.1
 * @date 2021-03-31
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "dbConnection.h"

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio.hpp>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <memory>
#include <string>

namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;

namespace NSMyProgramState
{
    std::size_t requestCount()
    {
        static std::size_t count = 0;
        return ++count;
    }

    std::time_t now()
    {
        return std::time(0);
    }

    /**
     * @brief connects do Postgree db, selects all users and returns data
     *        in http table format
     * 
     * @return std::string 
     */
    std::string getUsers()
    {
        NSDBConnection::PGConnection conn("34.65.88.18", "5432", "postgres", "postgres", "iDGJ6E7JlkJJ3u5v");
        auto res = conn.executeSQL("select * from users;");
        if (PQresultStatus(res) != PGRES_TUPLES_OK)
        {
            return PQresultErrorMessage(res);
        }
        else
        {
            // probably we have a leak in res.
            // TODO: run valgrind, release all acquired resources
            return NSDBConnection::htmlTableFromRes(res);
        }
    }
}

class HTTPConnection : public std::enable_shared_from_this<HTTPConnection>
{
public:
    HTTPConnection(tcp::socket socket)
        : socket_(std::move(socket))
    {}

    // Initiate the asynchronous operations associated with the connection.
    void start()
    {
        readRequest();
        checkDeadline();
    }

private:
    // The socket for the currently connected client.
    tcp::socket socket_;

    // The buffer for performing reads.
    beast::flat_buffer buffer_{8192};

    // The request message.
    http::request<http::dynamic_body> request_;

    // The response message.
    http::response<http::dynamic_body> response_;

    // The timer for putting a deadline on connection processing.
    net::steady_timer deadline_{
        socket_.get_executor(), std::chrono::seconds(60)};

    // Asynchronously receive a complete request message.
    void readRequest()
    {
        auto self = shared_from_this();

        http::async_read(socket_, buffer_, request_,
                        [self](beast::error_code ec, std::size_t bytes_transferred)
                        {
                            boost::ignore_unused(bytes_transferred);
                            if(!ec)
                                self->processRequest();
                        });
    }

    // Determine what needs to be done with the request message.
    void processRequest()
    {
        response_.version(request_.version());
        response_.keep_alive(false);

        switch(request_.method())
        {
            case http::verb::get:
                response_.result(http::status::ok);
                response_.set(http::field::server, "Beast");
                createResponse();
                break;

            default:
                // We return responses indicating an error if
                // we do not recognize the request method.
                response_.result(http::status::bad_request);
                response_.set(http::field::content_type, "text/plain");
                beast::ostream(response_.body())
                    << "Invalid request-method '"
                    << std::string(request_.method_string())
                    << "'";
                break;
        }

        writeResponse();
    }

    // Construct a response message based on the program state.
    void createResponse()
    {
        if(request_.target() == "/count")
        {
            response_.set(http::field::content_type, "text/html");
            beast::ostream(response_.body())
                << "<html>\n"
                <<  "<head><title>Request count</title></head>\n"
                <<  "<body>\n"
                <<  "<h1>Request count</h1>\n"
                <<  "<p>There have been "
                <<  NSMyProgramState::requestCount()
                <<  " requests so far.</p>\n"
                <<  "</body>\n"
                <<  "</html>\n";
        }
        else if(request_.target() == "/time")
        {
            response_.set(http::field::content_type, "text/html");
            beast::ostream(response_.body())
                <<  "<html>\n"
                <<  "<head><title>Current time</title></head>\n"
                <<  "<body>\n"
                <<  "<h1>Current time</h1>\n"
                <<  "<p>The current time is "
                <<  NSMyProgramState::now()
                <<  " seconds since the epoch.</p>\n"
                <<  "</body>\n"
                <<  "</html>\n";
        }
        else if (request_.target() == "/users")
        {
            response_.set(http::field::content_type, "text/html");
            beast::ostream(response_.body())
                <<  "<html>\n"
                <<  "<head><title>Users</title></head>\n"
                <<  "<body>\n"
                <<  "<h1>The list of users in DB</h1>\n"
                <<  "<p>The result of getUsers() calls  "
                <<  NSMyProgramState::getUsers()
                <<  "</p>\n"
                <<  "</body>\n"
                <<  "</html>\n";
        }
        else
        {
            response_.result(http::status::not_found);
            response_.set(http::field::content_type, "text/html");
            beast::ostream(response_.body())
                 <<  "<html>\n"
                <<  "<head><title>Main page</title></head>\n"
                <<  "<body>\n"
                <<  "<h1>Implemented endpoints:</h1>\n"
                <<  "<p><a href=\"count\">How many requests has been procced</a></p>\n"
                <<  "<p><a href=\"time\">Server's time</a></p>\n"
                <<  "<p><a href=\"users\">List of users in DB</a></p>\n"
                <<  "</body>\n"
                <<  "</html>\n";
        }
    }

    // Asynchronously transmit the response message.
    void writeResponse()
    {
        auto self = shared_from_this();

        response_.content_length(response_.body().size());

        http::async_write(
            socket_,
            response_,
            [self](beast::error_code ec, std::size_t)
            {
                self->socket_.shutdown(tcp::socket::shutdown_send, ec);
                self->deadline_.cancel();
            });
    }

    // Check whether we have spent enough time on this connection.
    void checkDeadline()
    {
        auto self = shared_from_this();

        deadline_.async_wait(
            [self](beast::error_code ec)
            {
                if(!ec)
                {
                    // Close socket to cancel any outstanding operation.
                    self->socket_.close(ec);
                }
            });
    }
};

// "Loop" forever accepting new connections.
void HTTPServer(tcp::acceptor& acceptor, tcp::socket& socket)
{
  acceptor.async_accept(socket,
      [&](beast::error_code ec)
      {
          if(!ec)
              std::make_shared<HTTPConnection>(std::move(socket))->start();
          HTTPServer(acceptor, socket);
      });
}

int main(int argc, char* argv[])
{
    try
    {
        // Check command line arguments.
        if(argc != 3)
        {
            std::cerr << "Usage: " << argv[0] << " <address> <port>\n";
            std::cerr << "  For IPv4, try:\n";
            std::cerr << "    receiver 0.0.0.0 80\n";
            std::cerr << "  For IPv6, try:\n";
            std::cerr << "    receiver 0::0 80\n";
            return EXIT_FAILURE;
        }

        auto const address = net::ip::make_address(argv[1]);
        unsigned short port = static_cast<unsigned short>(std::atoi(argv[2]));

        net::io_context ioc{1};

        tcp::acceptor acceptor{ioc, {address, port}};
        tcp::socket socket{ioc};
        HTTPServer(acceptor, socket);

        ioc.run();
    }
    catch(std::exception const& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
