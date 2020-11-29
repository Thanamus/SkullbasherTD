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

#include "Component.hpp"


#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
// #include <sre/SDLRenderer.hpp>
#include <sre/impl/GL.hpp>

#include <sre/ModelImporter.hpp>
#include <glm/gtx/string_cast.hpp>
#include <sre/Inspector.hpp>


class WorldObject : public Component  {
public:
    explicit WorldObject(GameObject* gameObject);

    ~WorldObject();

    void setBuildable(bool isBuildable);
    void setPath(bool isPath);

    bool getBuildableStatus();
    bool getPathStatus();


private:
    bool isbuildable = false;
    bool isPath = false;
};

