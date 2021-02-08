#pragma once

#include "sre/SDLRenderer.hpp"
#include "sre/Material.hpp"

struct TowerPart {
    std::string name;
    std::string model;
    glm::vec3 position;
    glm::vec3 scale;
    glm::vec3 rotation;
    std::vector<TowerPart> parts;
};

class Tower
{
public:
    explicit Tower(
//            int id,
//            std::string name,
//            const std::string& icon,
//            std::string model,
//            int buildCost,
//            float constructionTime,
//            float delay,
//            float launchTime,
//            float reloadTime,
//            float projectileAirTime,
//            float damage,
//            glm::vec3 position,
//            glm::vec3 scale,
//            glm::vec3 rotation,
//            std::vector<TowerPart> parts
    );

    int getId() const;
    const std::string &getName() const;
    const std::shared_ptr<sre::Texture> &getIcon() const;
    const std::string &getModel() const;
    int getBuildCost() const;
    float getConstructionTime() const;
    float getDelay() const;
    float getLaunchTime() const;
    float getReloadTime() const;
    float getProjectileAirTime() const;
    float getDamage() const;
    const glm::vec3 &getPosition() const;
    const glm::vec3 &getScale() const;
    const glm::vec3 &getRotation() const;
    const std::vector<TowerPart> &getParts() const;

    void setId(int id);

    void setName(const std::string &name);

    void setIcon(const std::shared_ptr<sre::Texture> &icon);

    void setModel(const std::string &model);

    void setBuildCost(int buildCost);

    void setConstructionTime(float constructionTime);

    void setDelay(float delay);

    void setLaunchTime(float launchTime);

    void setReloadTime(float reloadTime);

    void setProjectileAirTime(float projectileAirTime);

    void setDamage(float damage);

    void setPosition(const glm::vec3 &position);

    void setScale(const glm::vec3 &scale);

    void setRotation(const glm::vec3 &rotation);

    void setParts(const std::vector<TowerPart> &parts);

    const std::string &getIndicator() const;

    void setIndicator(const std::string &indicator);

private:
    int id{};
    std::string name;
    std::shared_ptr<sre::Texture> icon;
    std::string indicator;
    std::string model;
    int buildCost{};
    float constructionTime{};
    float delay{};
    float launchTime{};
    float reloadTime{};
    float projectileAirTime{};
    float damage{};
    glm::vec3 position{};
    glm::vec3 scale{};
    glm::vec3 rotation{};
    std::vector<TowerPart> parts;
};
