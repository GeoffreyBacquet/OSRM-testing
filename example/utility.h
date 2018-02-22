#ifndef UTILITY_H
#define UTILITY_H
#include <boost/property_tree/ptree.hpp>
#include <vector>

class Utility
{
public:

    static boost::property_tree::ptree build(std::vector<std::vector<std::string>> routestr,boost::property_tree::ptree tri);

};
#endif // UTILITY
