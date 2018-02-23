#include "jsonextractor.h"

std::vector<std::string> JSonExtractor::extractFromResult(osrm::util::json::Object result)
{
    std::vector<std::string> gpsstr;

    auto &trip = result.values[ "trips" ].get< osrm::json::Array >()
            .values.at( 0 ).get< osrm::json::Object >();

    const auto distance = trip.values[ "distance" ].get< osrm::json::Number >().value;
    const auto duration = trip.values[ "duration" ].get< osrm::json::Number >().value;

    // Warn users if extract does not contain the default coordinates from above
    if ( distance == 0 || duration == 0 )
    {
        std::cout << "Note: distance or duration is zero. ";
        std::cout << "You are probably doing a query outside of the OSM extract.\n\n";
    }

    auto &steps = trip.values[ "legs" ].get< osrm::json::Array >()
            .values.at( 0 ).get< osrm::json::Object >()
            .values[ "steps" ].get< osrm::json::Array >();

    for( size_t j = 0; j < steps.values.size(); j++ )
    {
        auto location = steps.values.at( j ).get< osrm::json::Object >()
                .values[ "maneuver" ].get< osrm::json::Object >()
                .values[ "location" ].get< osrm::json::Array >();

        float lat = location.values.at( 0 ).get< osrm::json::Number >().value;
        float lon = location.values.at( 1 ).get< osrm::json::Number >().value;

        std::string temp = std::to_string(lat) + "," + std::to_string(lon);

        gpsstr.push_back( temp );
    }
    std::reverse( gpsstr.begin(), gpsstr.end());

    return gpsstr;
}
