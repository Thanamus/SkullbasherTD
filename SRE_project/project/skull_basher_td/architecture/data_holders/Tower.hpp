#pragma once

#include "sre/SDLRenderer.hpp"
#include "sre/Material.hpp"
#include "../components/rendering/Model.hpp"
#include "../components/animation/Animation.hpp"

struct TowerProjectile {
    std::shared_ptr<Model> model;
    float damage;
    float mass;
    glm::vec3 position;
    glm::vec3 scale;
    glm::vec3 rotation;
    std::string hitboxType;
    glm::vec3 hitboxSize;
    float radius;
};

struct TowerPart {
    std::string name;
    std::shared_ptr<Model> model;
    glm::vec3 position;
    glm::vec3 scale;
    glm::vec3 rotation;
    std::vector<TowerPart> parts;
    std::map<std::string, std::shared_ptr<Animation>> animations;
};


class Tower
{
public:
    explicit Tower();

    int getId() const;
    const std::string &getName() const;
    const std::shared_ptr<sre::Texture> &getIcon() const;
    std::shared_ptr<Model> getModel() const;
    int getBuildCost() const;
    float getConstructionTime() const;
    float getDelay() const;
    float getLaunchTime() const;
    float getReloadTime() const;
    float getProjectileAirTime() const;
    const glm::vec3 &getPosition() const;
    const glm::vec3 &getScale() const;
    const glm::vec3 &getRotation() const;
    const std::vector<TowerPart> &getParts() const;

    void setId(int id);
    void setName(const std::string &name);
    void setIcon(const std::shared_ptr<sre::Texture> &icon);
    void setModel(const std::shared_ptr<Model> &model);
    void setBuildCost(int buildCost);
    void setConstructionTime(float constructionTime);
    void setDelay(float delay);
    void setLaunchTime(float launchTime);
    void setReloadTime(float reloadTime);
    void setProjectileAirTime(float projectileAirTime);
    void setPosition(const glm::vec3 &position);
    void setScale(const glm::vec3 &scale);
    void setRotation(const glm::vec3 &rotation);
    void setParts(const std::vector<TowerPart> &parts);

    std::shared_ptr<Model> getIndicator() const;

    void setIndicator(const std::shared_ptr<Model> &indicator);

    const TowerProjectile &getProjectile() const;

    void setProjectile(const TowerProjectile &projectile);

    float getRange() const;

    void setRange(float range);

    const std::map<std::string, std::shared_ptr<Animation>> &getAnimations() const;

    void setAnimations(const std::map<std::string, std::shared_ptr<Animation>> &animations);

private:
    int id{};
    std::string name;
    std::shared_ptr<sre::Texture> icon;
    std::shared_ptr<Model> indicator;
    std::shared_ptr<Model> model;
    int buildCost{};
    float constructionTime{};
    float delay{};
    float range{};
    float launchTime{};
    float reloadTime{};
    float projectileAirTime{};
    TowerProjectile projectile;
    glm::vec3 position{};
    glm::vec3 scale{};
    glm::vec3 rotation{};
    std::vector<TowerPart> parts;
    std::map<std::string, std::shared_ptr<Animation>> animations;
};
