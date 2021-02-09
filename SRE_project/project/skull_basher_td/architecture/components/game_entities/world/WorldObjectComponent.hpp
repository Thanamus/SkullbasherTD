#pragma once

#include <iostream>
#include <vector>
#include <fstream>

#include "sre/SDLRenderer.hpp"
#include "sre/Material.hpp"
#include "sre/Texture.hpp"
#include "sre/Renderer.hpp"

// #include "WorldMap.hpp"
// #include "FirstPersonController.hpp"

#include "../../Component.hpp"


#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
// #include <sre/SDLRenderer.hpp>
#include <sre/impl/GL.hpp>

#include <sre/ModelImporter.hpp>
#include <glm/gtx/string_cast.hpp>
#include <sre/Inspector.hpp>


/*
    WorldObjectComponent is a simple component class that holds some extra information on the map tiles that make up the levels
*/


class WorldObjectComponent : public Component  {
public:
    explicit WorldObjectComponent(GameObject* gameObject);

    ~WorldObjectComponent();

    void setBuildable(bool isBuildable);
    void setIsPath(bool isPath);

    bool getBuildableStatus();
    bool getPathStatus();


private:
    bool isbuildable = false; // determines if the tile can be built on
    bool isPath = false; // determines if the tile is part of the path
};

