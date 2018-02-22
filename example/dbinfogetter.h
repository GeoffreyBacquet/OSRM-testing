#ifndef STARTPOINT_H
#define STARTPOINT_H

#include <vector>
#include <map>
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

class Dbinfogetter
{
public:
    Dbinfogetter(std::string address, std::string adminusername, std::string password, std::string usedDB, std::string usedtable);
    std::vector< std::vector< float > > startpoint() const;

private:
    std::vector< std::vector< float > > m_startpoint;
    std::string m_address;
    std::string m_adminusername;
    std::string m_password;
    std::string m_usedDB;
    std::string m_usedtable;
};

#endif // STARTPOINT_H
