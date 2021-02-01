#pragma once

#include "sre/SDLRenderer.hpp"
#include "sre/Material.hpp"

class Tower
{
public:
    explicit Tower(
            int id,
            std::string name,
            std::string icon,
            std::string mesh,
            int buildCost,
            float constructionTime,
            float delay
    );

    int getId() const;

    const std::string &getName() const;

    const std::shared_ptr<sre::Texture> &getIcon() const;

    const std::string &getMesh() const;

    int getBuildCost() const;

    float getConstructionTime() const;

    float getDelay() const;

private:
    int id;
    std::string name;
    std::shared_ptr<sre::Texture> icon;
    std::string mesh;
    int buildCost;
    float constructionTime;
    float delay;
};
