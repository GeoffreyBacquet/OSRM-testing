#ifndef DBINFOGETTER_H
#define DBINFOGETTER_H

#include <vector>

#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

class DbInfoGetter
{
public:
    DbInfoGetter( std::string address, std::string adminusername, std::string password, std::string usedDB, std::string usedtable );
    std::vector< std::vector< float > > startpoint() const;

private:
    std::vector< std::vector< float > > m_startpoint;
    std::string m_address;
    std::string m_adminusername;
    std::string m_password;
    std::string m_usedDB;
    std::string m_usedtable;
};

#endif // DBINFOGETTER_H


//need to work on this to make it static and fully customizable (ip address, request, table...etc)
