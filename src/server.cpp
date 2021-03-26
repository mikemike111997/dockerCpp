#include "server.h"
#include <ctime>

namespace my_program_state
{
    std::size_t request_count()
    {
        static std::size_t count = 0;
        return ++count;
    }

    std::time_t now()
    {
        return std::time(0);
    }
}

void HttpConnection::start()
{
    read_request();
    check_deadline();
}


void HttpConnection::read_request()
{
    auto self = shared_from_this();

    http::async_read(
        socket_,
        buffer_,
        request_,
        [self](beast::error_code ec,
            std::size_t bytes_transferred)
        {
            boost::ignore_unused(bytes_transferred);
            if(!ec)
                self->process_request();
        });
}

void HttpConnection::process_request()
{
    response_.version(request_.version());
    response_.keep_alive(false);

    switch(request_.method())
    {
    case http::verb::get:
        response_.result(http::status::ok);
        response_.set(http::field::server, "Beast");
        create_response();
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

    write_response();
}

// Construct a response message based on the program state.
void HttpConnection::create_response()
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
            <<  my_program_state::request_count()
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
            <<  my_program_state::now()
            <<  " seconds since the epoch.</p>\n"
            <<  "</body>\n"
            <<  "</html>\n";
    }
    else
    {
        response_.result(http::status::not_found);
        response_.set(http::field::content_type, "text/plain");
        beast::ostream(response_.body()) << "File not found\r\n";
    }
}

// Asynchronously transmit the response message.
void HttpConnection::write_response()
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
void HttpConnection::check_deadline()
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
