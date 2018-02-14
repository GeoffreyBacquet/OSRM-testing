#ifndef STARTPOINT_H
#define STARTPOINT_H

#include <vector>
#include "mysql_connection.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

class Startpoint
{
public:
    Startpoint();

    std::vector< std::vector< float > > startpoint() const;

    static void affichage( std::vector< std::vector< std::vector< float > > > gps );

    static void insert();

private:
    std::vector< std::vector< float > > m_startpoint;
};

#endif // STARTPOINT_H
