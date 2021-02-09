#pragma once

#include "sre/SDLRenderer.hpp"
#include "sre/Material.hpp"

struct TowerProjectile {
    std::string model;
    float damage;
    float mass;
    glm::vec3 position;
    glm::vec3 scale;
    glm::vec3 rotation;
    std::string hitboxType;
    glm::vec3 hitboxSize;
    float radius;
};

struct TowerAnimationFrame {
    float duration;
    glm::vec3 translate;
    glm::vec3 scale;
    glm::vec3 rotate;
};

struct TowerAnimation {
    std::string name;
    bool looping;
    std::vector<TowerAnimationFrame> frames;
};

struct TowerPart {
    std::string name;
    std::string model;
    glm::vec3 position;
    glm::vec3 scale;
    glm::vec3 rotation;
    std::vector<TowerPart> parts;
    std::vector<TowerAnimation> animations;
};


class Tower
{
public:
    explicit Tower();

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
    void setPosition(const glm::vec3 &position);
    void setScale(const glm::vec3 &scale);
    void setRotation(const glm::vec3 &rotation);
    void setParts(const std::vector<TowerPart> &parts);

    const std::string &getIndicator() const;

    void setIndicator(const std::string &indicator);

    const TowerProjectile &getProjectile() const;

    void setProjectile(const TowerProjectile &projectile);

    float getRange() const;

    void setRange(float range);

    const std::vector<TowerAnimation> &getAnimations() const;

    void setAnimations(const std::vector<TowerAnimation> &animations);

private:
    int id{};
    std::string name;
    std::shared_ptr<sre::Texture> icon;
    std::string indicator;
    std::string model;
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
    std::vector<TowerAnimation> animations;
};
