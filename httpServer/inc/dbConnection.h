#pragma once

/**
 * @file dbConnection.h
 * @author Mykhailo Lohvynenko (mikemike111997@gmail.com)
 * @brief Wrapper class for SQL interaction
 * @version 0.1
 * @date 2021-04-04
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <string>
#include <string_view>
#include <libpq-fe.h>
#include <memory>


namespace NSDBConnection
{
    class PGConnection final
    {
    public:
        PGConnection(std::string host, std::string port, std::string dbName,
                    std::string dbUser, std::string dbPass);

        PGConnection(const PGConnection&) = delete;
        
        PGConnection& operator=(const PGConnection&) = delete;

        ~PGConnection() = default;
        
        std::shared_ptr<PGconn> connection() const { return connection_; }

        /**
         * @brief executes a given SQL command and returns a result
         * 
         * @param sqlStr SQL to be exucuted
         * @return PGresult* 
         */
        PGresult* executeSQL(std::string_view sqlStr);

    private:
        void connect(); 

    private:
        std::string host_;

        std::string port_;
        
        std::string dbName_;
        
        std::string dbUser_;
        
        std::string dbPass_;

        std::shared_ptr<PGconn> connection_;
    };

    /**
     * @brief composes an html table tag using the retrieved data. 
     * @param res contains fethed data from the PostgreSQL DB 
     * @return std::string 
     */
    std::string htmlTableFromRes(PGresult* res);
}
