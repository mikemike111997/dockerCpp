#include <iostream>
#include "server.h"



static void http_server(tcp::acceptor& acceptor, tcp::socket& socket)
{
  acceptor.async_accept(socket,
      [&](beast::error_code ec)
      {
          if(!ec)
              std::make_shared<HttpConnection>(std::move(socket))->start();
          http_server(acceptor, socket);
      });
}

int main(int argC, char* argV[])
{
    std::cout << argV[0] << " has been started!" << std::endl;

    try
    {

        auto const address = net::ip::make_address("0.0.0.0");
        unsigned short port = 80;

        net::io_context ioc{1};

        tcp::acceptor acceptor{ioc, {address, port}};
        tcp::socket socket{ioc};
        http_server(acceptor, socket);

        ioc.run();
    }
    catch(std::exception const& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return 0;
}
