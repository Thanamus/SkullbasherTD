//
// Created by Morten Nobel-Jørgensen on 29/09/2017.
//

#include <sre/Renderer.hpp>
#include "sre/SpriteAtlas.hpp"
#include "Tower.hpp"


#define GLM_ENABLE_EXPERIMENTAL

using namespace sre;
using namespace glm;

Tower::Tower(int id,
             std::string name,
             std::string icon,
             std::string mesh,
             int buildCost,
             float constructionTime,
             float delay)
{
    this->id = id;
    this->name = name;
    this->icon = Texture::create().withFile("assets/" + icon).withFilterSampling(false).build();
    this->mesh = mesh;
    this->buildCost = buildCost;
    this->constructionTime = constructionTime;
    this->delay = delay;
}

int Tower::getId() const {
    return id;
}

const std::string &Tower::getName() const {
    return name;
}

const std::shared_ptr<sre::Texture> &Tower::getIcon() const {
    return icon;
}

const std::string &Tower::getMesh() const {
    return mesh;
}

int Tower::getBuildCost() const {
    return buildCost;
}

float Tower::getConstructionTime() const {
    return constructionTime;
}

float Tower::getDelay() const {
    return delay;
}
