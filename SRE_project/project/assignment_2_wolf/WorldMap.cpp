//
// Created by Morten Nobel-Jørgensen on 22/09/2017.
//

#include "WorldMap.hpp"
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include <fstream>
#include <iostream>

using namespace std;

void WorldMap::loadMap(std::string filename)
{
    using namespace rapidjson;
    ifstream fis(filename);
    IStreamWrapper isw(fis);
    Document d;
    d.ParseStream(isw);

    // get tilemap
    auto tileMap = d["tileMap"].GetArray();
    for (auto &tile : tileMap)
    {
        std::vector<int> tileArr;
        for (auto &value : tile.GetArray())
        {
            tileArr.push_back(value.GetInt());
        }
        values.push_back(tileArr);
    }

    // get starting camera stuff
    startingPosition.x = d["spawn"]["x"].GetFloat();
    startingPosition.y = d["spawn"]["y"].GetFloat();
    startingRotation = d["spawn"]["angle"].GetFloat();

    // set floor/ceiling colours
    auto floorColorValues = d["floorColor"].GetArray();
    floorColor = glm::vec4(
        floorColorValues[0].GetFloat(),
        floorColorValues[1].GetFloat(),
        floorColorValues[2].GetFloat(),
        floorColorValues[3].GetFloat());

    auto ceilColorValues = d["ceilColor"].GetArray();
    ceilColor = glm::vec4(
        ceilColorValues[0].GetFloat(),
        ceilColorValues[1].GetFloat(),
        ceilColorValues[2].GetFloat(),
        ceilColorValues[3].GetFloat());
}

int WorldMap::getTile(int x, int y)
{
    return values.at(y).at(x);
}

int WorldMap::getWidth()
{
    return values[0].size();
}

int WorldMap::getHeight()
{
    return values.size();
}

glm::vec2 WorldMap::getStartingPosition()
{
    return startingPosition;
}

float WorldMap::getStartingRotation()
{
    return startingRotation;
}

glm::vec4 WorldMap::getFloorColor()
{
    return floorColor;
}

glm::vec4 WorldMap::getCeilColor()
{
    return ceilColor;
}
