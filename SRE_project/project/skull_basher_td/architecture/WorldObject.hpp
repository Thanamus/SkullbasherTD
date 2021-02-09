#pragma once

#include "Component.hpp"

/*
    WorldObject is a simple component class that holds some extra information on the map tiles that make up the levels
*/

class GameObject;

class WorldObject : public Component  {
public:
    explicit WorldObject(GameObject* gameObject);

    ~WorldObject();

    void setBuildable(bool isBuildable);
    void setIsPath(bool isPath);

    bool getBuildableStatus();
    bool getPathStatus();


private:
    bool isbuildable = false; // determines if the tile can be built on
    bool isPath = false; // determines if the tile is part of the path
};

