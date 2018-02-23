#ifndef JSONEXTRACTOR_H
#define JSONEXTRACTOR_H
#include <vector>
#include <algorithm>
#include <iostream>
#include "osrm/json_container.hpp"

class JSonExtractor
{
public:
    static std::vector<std::string> extractFromResult(osrm::json::Object result);
};

#endif // JSONEXTRACTOR
