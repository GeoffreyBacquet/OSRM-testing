#include "dbinfogetter.h"

Dbinfogetter::Dbinfogetter(std::string address, std::string adminusername, std::string password, std::string usedDB,std::string usedtable)
    :m_address(address), m_adminusername(adminusername), m_password(password), m_usedDB(usedDB), m_usedtable(usedtable)
{
    //Creating object for:
    sql::Driver *driver;
    sql::Connection *con; //connection
    sql::Statement *stmt; //request
    sql::ResultSet *res; //response

    driver = get_driver_instance();
    con = driver->connect(m_address, m_adminusername, m_password); //(address, admin username, password)
    stmt = con->createStatement();
    stmt->execute( "USE " + m_usedDB ); //DB to use

    res= stmt->executeQuery( "SELECT * FROM " + m_usedtable ); //Request

    while( res->next() ) //until we reach the end of result
    {
        float lat = res->getDouble( "latitude" );
        float lon = res->getDouble( "longitude" );
        m_startpoint.push_back( { lon, lat } );
    }

    delete res; //destructing response
    delete stmt; //then request
    delete con; //finaly connection
}

std::vector< std::vector< float > > Dbinfogetter::startpoint() const
{
    return m_startpoint;
}
