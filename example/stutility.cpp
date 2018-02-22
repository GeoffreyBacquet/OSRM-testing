#include "stutility.h"

boost::property_tree::ptree Stutility::build(std::vector<std::vector<std::__cxx11::string> > routestr, boost::property_tree::ptree tri)
{
    for(size_t i = 0; i < routestr.size(); i++)
    {
        std::string path = routestr[0][0]; //root of tree
        for(size_t j = 1; j < routestr[i].size(); j++)
        {
            path = path + "/" + routestr[i][j]; //constructing tree

        }
        tri.add(boost::property_tree::ptree::path_type(path, '/'), "departure"); //adding value departure with custom path
    }
    return tri;
}

void Stutility::saveasjson(std::string filename, boost::property_tree::ptree tri)
{
    boost::property_tree::json_parser::write_json(filename, tri);
}
