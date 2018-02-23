#include "stutility.h"

boost::property_tree::ptree Stutility::build(std::vector<std::vector<std::__cxx11::string> > routestr)
{
    boost::property_tree::ptree tri;
    int cptr( 0 );
    for(size_t i = 0; i < routestr.size(); i++)
    {
        std::string path = routestr[0][0]; //root of tree
        for(size_t j = 1; j < routestr[i].size(); j++)
        {
            path = path + "/" + routestr[i][j]; //constructing tree

        }
        boost::property_tree::ptree temp;
        temp.put("idtrip", std::to_string(i));
        temp.put("Fumeur?", "non");
        temp.put("Blabla?", "Beaucoup");
        temp.put("Heure arrivée", "8h00");
        temp.put("Vehicule", "Hyundai i20");
//        tri.add(boost::property_tree::ptree::path_type(path, '/'), "departure " + std::to_string(cptr++) ); //adding value departure with custom path
        tri.add_child(boost::property_tree::ptree::path_type(path, '/'), temp);
    }
    return tri;
}

void Stutility::saveasjson(std::string filename, boost::property_tree::ptree tri)
{
    boost::property_tree::json_parser::write_json(filename, tri);
}
