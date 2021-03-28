#include <iostream>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/lexical_cast.hpp>

using boost::asio::ip::tcp;


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
    try
    {
        boost::asio::io_context io;
        constexpr unsigned short port = 2048;
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
