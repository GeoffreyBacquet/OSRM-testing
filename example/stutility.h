#ifndef STUTILITY_H
#define STUTILITY_H
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <vector>

class Stutility
{
public:

    static boost::property_tree::ptree build(std::vector<std::vector<std::string>> routestr,boost::property_tree::ptree tri);
    static void saveasjson(std::string filename, boost::property_tree::ptree tri);

};
#endif // STUTILITY
