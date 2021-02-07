//
// Created by kb on 27/01/2021.
//

#ifndef SIMPLERENDERENGINEPROJECT_TOWERBEHAVIOURCOMPONENT_HPP
#define SIMPLERENDERENGINEPROJECT_TOWERBEHAVIOURCOMPONENT_HPP

#include "Scriptable.hpp"
#include "Component.hpp"
#include "GameObject.hpp"
#include "Targetable.hpp"
#include "glm/glm.hpp"

class TowerBehaviourComponent : public Component, public Scriptable, Targetable {
    using Scriptable::Scriptable;
public:
    enum TowerAction {
        TB_TARGETING,
        TB_AIMING,
        TB_SHOOTING,
        TB_RELOADING
    };

    explicit TowerBehaviourComponent(GameObject* gameObject);
    void update(float deltaTime) override;

    float getRange() const;
    void setRange(float _range);

    bool isReadyToShoot() const;
    void setReadyToShoot(bool readyToShoot);

    float getProjectileAirTime() const;
    void setProjectileAirTime(float projectileAirTime_);

    float getReloadSpeed() const;
    void setReloadSpeed(float reloadSpeed_);

    const glm::vec3 &getAimPos() const;
    void setAimPos(const glm::vec3 &aimPos_);

    bool targetInRange() const;

private:
    glm::vec3 aimPos = glm::vec3(-1);
    glm::vec3 projectileStart = glm::vec3(0);

    // simply maps enum actions to strings
    std::map<TowerAction, std::string> actions;

    // attributes
    // TODO: shift to reading from json once it's all done
    float range = 10.f;
    bool readyToShoot = true;
    float projectileAirTime = 2.f;
    float reloadSpeed = 3.f;

    void shoot(float deltaTime);
    static bool inCircle(glm::vec2 point, glm::vec2 center, float radius) ;
};


#endif //SIMPLERENDERENGINEPROJECT_TOWERBEHAVIOURCOMPONENT_HPP
