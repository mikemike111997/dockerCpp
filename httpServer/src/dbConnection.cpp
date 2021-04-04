/**
 * @file dbConnection.cpp
 * @author Mykhailo Lohvynenko (mikemike111997@gmail.com)
 * @brief Implements PG db connection class
 * @version 0.1
 * @date 2021-04-04
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "dbConnection.h"
#include <stdexcept>

NSDBConnection::PGConnection::PGConnection(
    std::string host, std::string port, std::string dbName,
    std::string dbUser, std::string dbPass)
    : host_(std::move(host)), port_(std::move(port)), dbName_(std::move(dbName_)),
      dbUser_(std::move(dbUser)), dbPass_(std::move(dbPass))                         
{
    connect();
}

PGresult* NSDBConnection::PGConnection::executeSQL(std::string_view sqlStr)
{
    PQsendQuery( connection_.get(), sqlStr.data() );
    return PQgetResult(connection_.get());
}

void NSDBConnection::PGConnection::connect()
{
    connection_.reset( PQsetdbLogin(host_.c_str(), port_.c_str(), nullptr, nullptr,
                                    dbName_.c_str(), dbUser_.c_str(), dbPass_.c_str()),
                       &PQfinish );
    if (PQstatus( connection_.get() ) != CONNECTION_OK &&
        PQsetnonblocking(connection_.get(), 1) != 0 )
    {
        throw std::runtime_error( PQerrorMessage( connection_.get() ) );
    }
}

std::string NSDBConnection::htmlTableFromRes(PGresult* res)
{
    std::string resStr = "<table><tr>";
    
    for (int h = 0; h < PQnfields(res); ++h)
    {
        resStr.append("<th>").append(PQfname(res, h)).append("</th>");
    }
    resStr.append("</tr>");

    for (int i = 0; i < PQntuples(res); ++i)
    {
        resStr.append("<tr>");
        for (int j = 0; j < PQnfields(res); ++j)
        {
            resStr.append("<td>").append(PQgetvalue(res, i, j)).append("</td>");
        }
        resStr.append("</tr>");
    }
    return resStr.append("</table>");
}