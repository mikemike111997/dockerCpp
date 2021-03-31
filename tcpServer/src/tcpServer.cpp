/**
 * @file tcpServer.cpp
 * @author Mykhailo Lohvynenko (mikemike111997@gmail.com)
 * @brief A simple TCP Server using boost::asio tutorial as a reference
 * @version 0.1
 * @date 2021-03-28
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <iostream>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/lexical_cast.hpp>

using boost::asio::ip::tcp;


/**
 * @brief generates an HTML page that contains info 
 *        about num of requests that has been processed
 * 
 * @return std::string
 */
static std::string generateMessage()
{
    static size_t count{0};
    std::ostringstream os;
    os  << "<html>\n"
        <<  "<head><title>Request count</title></head>\n"
        <<  "<body>\n"
        <<  "<h1>Request count</h1>\n"
        <<  "<p>There have been "
        <<  ++count
        <<  " requests so far.</p>\n"
        <<  "</body>\n"
        <<  "</html>\n";

    return os.str();
}


int main(int argC, char* argV[])
{
    std::cout << argV[0] << " has been started!" << std::endl;
    if (argC != 2)
    {
        std::cout << "invalid command, excpected: \"<executable> <port>\"" << std::endl;
        exit(-1);
    }

    try
    {
        boost::asio::io_context io;
        const unsigned short port = boost::lexical_cast<unsigned short>(argV[1]);
        tcp::acceptor acceptor(io, tcp::endpoint(tcp::v4(), port));
        for (;;)
        {
            tcp::socket socket(io);
            acceptor.accept(socket);

            boost::system::error_code ignoredError;
            const auto responseMsg = generateMessage();
            boost::asio::write(socket, boost::asio::buffer(responseMsg), ignoredError);
        }
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << std::endl;
        exit(-1);
    }

    std::cout << argV[0] << " has been stopped!" << std::endl;
    return 0;
}
