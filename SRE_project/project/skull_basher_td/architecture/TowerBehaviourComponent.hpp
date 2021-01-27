//
// Created by kb on 27/01/2021.
//

#ifndef SIMPLERENDERENGINEPROJECT_TOWERBEHAVIOURCOMPONENT_HPP
#define SIMPLERENDERENGINEPROJECT_TOWERBEHAVIOURCOMPONENT_HPP

#include "Scriptable.hpp"
#include "Component.hpp"
#include "glm/glm.hpp"

class TowerBehaviourComponent : public Component, Scriptable {
public:
    enum TowerAction {
        TB_TARGETING,
        TB_AIMING,
        TB_SHOOTING,
        TB_RELOADING
    };

    explicit TowerBehaviourComponent(GameObject* gameObject);
    void update();

    static std::vector<std::shared_ptr<GameObject>> getEnemyList;

private:
    TowerAction action;

    // attributes
    // TODO: shift to reading from json once it's all done
    float range = 200f;
    bool readyToShoot = true;
    float projectileSpeed = 2.f;
    float reloadSpeed = 3.f;

    // targeting stuff
    GameObject* target = nullptr;
    glm::vec3 aimPos = -1;

    // simple conversion from enum to string, done in this static method to ensure correspondence between action and
    // script
    static std::string getActionScriptName() {
        switch(action) {
            case TB_TARGETING:
                return "targeting";
            case TB_AIMING:
                return "aiming";
            case TB_SHOOTING:
                return "shooting";
            case TB_RELOADING:
                return "reloading";
            default:
                break;
        }
    };
};


#endif //SIMPLERENDERENGINEPROJECT_TOWERBEHAVIOURCOMPONENT_HPP
