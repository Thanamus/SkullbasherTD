//
// Created by kb on 27/01/2021.
//

#include "TowerBehaviourComponent.hpp"

TowerBehaviourComponent::TowerBehaviourComponent(GameObject* gameObject)
        : Component(gameObject) {
    loadScript(getActionScriptName(TB_TARGETING), "", true)
    loadScript(getActionScriptName(TB_AIMING), "", true)
    loadScript(getActionScriptName(TB_SHOOTING), "", true)
    loadScript(getActionScriptName(TB_RELOADING), "", true)
}

void TowerBehaviourComponent::update() {
    //first of all, tower reloads if needed
    if (!readyToShoot)
        action = TB_RELOADING;
    // if tower doesn't have a live target, get a new one
    else if(target == nullptr)
        action = TB_TARGETING;
    // if the tower is ready to shoot and has a target, but hasn't aimed yet, then calc where to shoot
    else if (aimPos == glm::vec3(-1))
        action = TB_AIMING;
    // if tower is ready and has calculated where to shoot, then shoot away!
    else if (aimPos != glm::vec3(-1) && readyToShoot)
        action = TB_SHOOTING;

    // run the correct action
    return run(getActionScriptName(action));
}
