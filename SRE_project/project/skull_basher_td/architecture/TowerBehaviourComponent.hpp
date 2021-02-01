//
// Created by kb on 27/01/2021.
//

#ifndef SIMPLERENDERENGINEPROJECT_TOWERBEHAVIOURCOMPONENT_HPP
#define SIMPLERENDERENGINEPROJECT_TOWERBEHAVIOURCOMPONENT_HPP

#include "Scriptable.hpp"
#include "Component.hpp"
#include "GameObject.hpp"
#include "glm/glm.hpp"

class TowerBehaviourComponent : public Component, public Scriptable {
    using Scriptable::Scriptable;
public:
    enum TowerAction {
        TB_TARGETING,
        TB_AIMING,
        TB_SHOOTING,
        TB_RELOADING
    };

    explicit TowerBehaviourComponent(GameObject* gameObject);
    void update() override;

    static std::vector<std::shared_ptr<GameObject>> getEnemyList;

    float getRange() const;
    void setRange(float _range);

    bool isReadyToShoot() const;
    void setReadyToShoot(bool readyToShoot);

    float getProjectileSpeed() const;
    void setProjectileSpeed(float _projectileSpeed);

    float getReloadSpeed() const;
    void setReloadSpeed(float _reloadSpeed);

    GameObject *getTarget() const;
    void setTarget(GameObject *_target);

    const glm::vec3 &getAimPos() const;
    void setAimPos(const glm::vec3 &_aimPos);

    bool targetInRange() const;

private:
    std::shared_ptr<GameManager> gameManager;
    glm::vec3 aimPos = glm::vec3(-1);

    // simply maps enum actions to strings
    std::map<TowerAction, std::string> actions;

    // attributes
    // TODO: shift to reading from json once it's all done
    float range = 200.f;
    bool readyToShoot = true;
    float projectileSpeed = 2.f;
    float reloadSpeed = 3.f;

    // targeting stuff
    GameObject* target = nullptr;

    static bool inCircle(glm::vec2 point, glm::vec2 center, float radius) ;
};


#endif //SIMPLERENDERENGINEPROJECT_TOWERBEHAVIOURCOMPONENT_HPP
