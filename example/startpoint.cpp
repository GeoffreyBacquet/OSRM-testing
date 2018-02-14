#include "startpoint.h"

Startpoint::Startpoint()
{
    //Creating object for:
    sql::Driver *driver;
    sql::Connection *con; //connection
    sql::Statement *stmt; //request
    sql::ResultSet *res; //response

    driver = get_driver_instance();
    con = driver->connect("localhost", "root", "iamroot"); //(address, admin username, password)
    stmt = con->createStatement();
    stmt->execute( "USE afpa_car_test" ); //DB to use

    res= stmt->executeQuery( "SELECT * FROM address" ); //Request

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

std::vector< std::vector< float > > Startpoint::startpoint() const
{
    return m_startpoint;
}


void Startpoint::affichage( std::vector< std::vector< std::vector< float > > > gps )
{
    for( size_t i = 0; i < gps.size() ; i++ )
    {
        //Travel
        std:: cout << "Travel: " << i+1 << "\n [";

        for( size_t j = 0; j < gps[ i ].size(); j++ )
        {
            //Step
            for( size_t k = 0; k < 2 ;k++ )
            {
                //GPS coordinate
                if( k == 0 )
                {
                std::cout << "{ " << gps[ i ][ j ][ k ] << ",";
                }
                else
                {
                    std::cout << gps[ i ][ j ][ k ] << " } ";
                }
            }
        }
        std::cout << "] \n";
    }
    std::cout << "\n";
}

std::map<std::string, int> Startpoint::makemap(std::vector<std::vector<std::string> > route)
{
    std::map< std::string, int > compteur;
    for( size_t i = 0; i < route.size(); i++ )
    {
        for( size_t j = 0; j < route[ i ].size(); j++ )
        {
            std::map< std::string, int >::iterator it;
            it = compteur.find( route[ i ][ j ] );

            if ( it != compteur.end() )
            {
                compteur[ route[ i ][ j ] ] += 1;
            }
            else
            {
                compteur[ route[ i ][ j ] ] = 1;
            }
        }
    }
    return compteur;
}

void Startpoint::insert()
{
    sql::Driver *driver;
    sql::Connection *con; //connection
    sql::PreparedStatement  *prep_stmt;
    driver = get_driver_instance();
    con = driver->connect("localhost", "root", "iamroot"); //(address, admin username, password)
    con->setSchema("afpa_car_test");
    prep_stmt = con->prepareStatement("INSERT INTO trip(address_departure_id) VALUES ( ?)");

    int i = 1;
    while( i < 1122)
    {

        prep_stmt->setInt(1, i);
        prep_stmt->execute();
        std::cout << "Ligne " << i << " inserée! \n ";
        i++;

    }
    delete prep_stmt; //then request
    delete con; //finaly connection

}
