#include "osrm/trip_parameters.hpp"

#include "osrm/coordinate.hpp"
#include "osrm/engine_config.hpp"
#include "osrm/json_container.hpp"

#include "osrm/osrm.hpp"
#include "osrm/status.hpp"

#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/json_parser.hpp"
#include "boost/foreach.hpp"

#include "startpoint.h"

#include <exception>
#include <iostream>
#include <string>
#include <utility>

#include <cstdlib>
#include <functional>
#include <set>

//boost::property_tree::ptree getTri( std::vector<std::vector<std::string>> routestr, boost::property_tree::ptree tri)
//{
//    for(size_t i = 0; i < routestr.size(); i++)
//    {
//        std::string path = routestr[i][0];
//        for(size_t j = 0; j < routestr[i].size(); j++)
//        {
//            if(j == routestr[i].size()-1)
//            {
//                tri.put(routestr[i][j], "end");
//            }
//            else
//            {
//                auto it = tri.find(routestr[i][j]);
//                if(it == tri.not_found())
//                {
//                    boost::property_tree::ptree tri2;
//                    tri2.add(routestr[i][j], "");
//                    tri.add_child(path, tri2);
//                }
//                path = path + "." + routestr[i][j];
//            }
//        }
//    }
//    return tri;
//}
boost::property_tree::ptree getTri( std::vector<std::vector<std::string>> routestr, boost::property_tree::ptree tri)
{
    int taillemax(0);

    for(size_t i=0; i < routestr.size(); i++)
    {
        if(routestr[i].size() > taillemax)
        {
            taillemax = routestr[i].size();
        }
    }

    for(size_t j=0; j < taillemax; j++)
    {
        std::string path = routestr[0][0];
        for(size_t k = 0; k < routestr.size(); k++)
        {
            if(j < routestr[k].size())
            {
                auto it = tri.find(routestr[k][j]);
                if(it == tri.not_found())
                {
                    boost::property_tree::ptree tri2;
                    tri2.put(routestr[k][j], "");
                    tri.add_child(path, tri2);
                }
            }
        }
    }
    return tri;
}

int main( int argc, char *argv[] )
{
    //check if .osrm is defined as argument when launching program
    if ( argc < 2 )
    {
        std::cerr << "Cannot find a .osrm path, Have you defined it on your project configuration? \n";
        return EXIT_FAILURE;
    }

    using namespace osrm;
    // Configure based on a .osrm base path, and no datasets in shared mem from osrm-datastore
    EngineConfig config;

    config.storage_config = { argv[ 1 ] };
    config.use_shared_memory = false;

    std::vector< std::vector< std::vector< float > > > route;
    std::vector< std::vector< std::string > > routestr;

    // - Multi-Level Dijkstra (MLD): requires extract+partition+customize pre-processing

    config.algorithm = EngineConfig::Algorithm::MLD;

    // Routing machine with several services (such as Route, Table, Nearest, Trip, Match)
    const OSRM osrm{ config };

    //Creating an object with all startpoint from database
    Startpoint coorGPS;

    // The following shows how to use the Route service; configure this service

    for( size_t i = 0; i < coorGPS.startpoint().size(); i++ )
    {
        TripParameters params;
        params.steps = true;
        params.overview = RouteParameters::OverviewType::False;

        std::vector< std::vector< float > > gps;
        std::vector< std::string > gpsstr;

        // Route for all start point

        params.coordinates.push_back( { util::FloatLongitude{ coorGPS.startpoint()[ i ][ 0 ] }, util::FloatLatitude{ coorGPS.startpoint()[ i ][ 1 ] } } );
        params.coordinates.push_back( { util::FloatLongitude{ 3.84468 }, util::FloatLatitude{ 43.5648 } } );

        // Response is in JSON format
        json::Object result;

        // Execute routing request, this does the heavy lifting
        const auto status = osrm.Trip( params, result );

        if ( status == Status::Ok )
        {
            auto &trips = result.values[ "trips" ].get< json::Array >();

            // Let's just use the first route
            auto &trip = trips.values.at( 0 ).get< json::Object >();
            const auto distance = trip.values[ "distance" ].get< json::Number >().value;
            const auto duration = trip.values[ "duration" ].get< json::Number >().value;
            auto &legs = trip.values[ "legs" ].get< json::Array >();
            auto &leg = legs.values.at( 0 ).get< json::Object >();
            auto &steps = leg.values[ "steps" ].get< json::Array >();

            // Warn users if extract does not contain the default coordinates from above
            if ( distance == 0 || duration == 0 )

            {
                std::cout << "Note: distance or duration is zero. ";
                std::cout << "You are probably doing a query outside of the OSM extract.\n\n";
            }

            //Constructing vector to build final map
            for( size_t j = 0; j < steps.values.size(); j++ )
            {
                auto &step = steps.values.at( j ).get< json::Object >();
                auto maneuver = step.values[ "maneuver" ].get< json::Object >();
                auto location = maneuver.values[ "location" ].get< json::Array >();
                float lat = location.values.at( 0 ).get< json::Number >().value;
                float lon = location.values.at( 1 ).get< json::Number >().value;
                std::string latstr = std::to_string(lat);
                size_t pos1 = latstr.find(".");
                latstr.erase(pos1, 1);
                std::string lonstr = std::to_string(lon);
                size_t pos2 = lonstr.find(".");
                lonstr.erase(pos2, 1);
                std::string temp = latstr + "," + lonstr;
                gpsstr.push_back( temp );
                gps.push_back( { lat, lon } );
            }
            std::reverse( gps.begin(), gps.end() );

        }

        else if ( status == Status::Error )
        {
            const auto code = result.values[ "code" ].get< json::String >().value;
            const auto message = result.values[ "message" ].get< json::String >().value;

            std::cout << "Code: " << code << "\n";
            std::cout << "Message: " << message << "\n";
        }
        routestr.push_back( gpsstr );
        route.push_back( gps );
    }

    //     Showing trip coordinate maneuver
    //    Startpoint::affichage( route );

    // Building map
    //        std::map< std::string, int > compteur(Startpoint::makemap(routestr));

    //Building set to sort map by value and not by key
    //        typedef std::function<bool( std::pair< std::string, int >, std::pair< std::string, int > ) > Comparator;

    //        Comparator compFunctor = []( std::pair< std::string, int > elem1 ,std::pair< std::string, int > elem2)
    //        {
    //            return elem1.second > elem2.second;
    //        };

    //        std::set< std::pair< std::string, int >, Comparator > setGPS( compteur.begin(), compteur.end(), compFunctor );

    //        //Showing set { lat , lon } : number
    //        for(std::pair< std::string, int > element : setGPS )
    //        {
    //            std::cout << element.first << " " << element.second << " passage \n";
    //        }
    boost::property_tree::ptree tri;

    tri = getTri( routestr, tri );
    boost::property_tree::json_parser::write_json(std::cout, tri);
}
