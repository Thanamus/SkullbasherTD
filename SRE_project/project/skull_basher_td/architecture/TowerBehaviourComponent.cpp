//
// Created by kb on 27/01/2021.
//

#include "TowerBehaviourComponent.hpp"
#include "Transform.hpp"
#include "Animator.hpp"
#include "EnemyComponent.hpp"
#include "Pathfinder.hpp"

TowerBehaviourComponent::TowerBehaviourComponent(GameObject* gameObject)
        : Component(gameObject) {

    gameManager = gameObject->getScene()->gameManager;
    lua.set_function("getGameObject", [&]()->GameObject* {
        return gameObject;
    });

    // extend GameObject definition
    // ugly but needed to use a template function
//   lua["usertype"]["GameObject"]["getTransform"] = &GameObject::getComponent<Transform>;
//    lua["usertype"]["GameObject"]["getAnimator"] = &GameObject::getComponent<Animator>;

    //Incomplete implementation of Animator
    auto animator_type = lua.new_usertype<Animator>( "Animator",
                                "setAnimationState", &Animator::setAnimationState,
                                "getAnimationState", &Animator::getAnimationState
    );

    //Incomplete implementation of Transform
    auto transform_type =lua.new_usertype<Transform>( "Transform",
                                 "globalTransform", &Transform::globalTransform,
                                 "lookAt", sol::overload(
                                         static_cast<void (Transform::*)(glm::vec3, glm::vec3)>(&Transform::lookAt),
                                         static_cast<void (Transform::*)(Transform*, glm::vec3)>(&Transform::lookAt)
                                         ),
                                 "globalPosition", &Transform::globalPosition
    );

    auto enemyComponent_type =lua.new_usertype<EnemyComponent>("EnemyComponent",
                                     "getPathfinder", &EnemyComponent::getPathfinder);

    auto pathfinder_type = lua.new_usertype<Pathfinder>("Pathfinder",
                                 "getCurrentPathIndex", &Pathfinder::getCurrentPathIndex);

    // workaround since lua and templates don't like each other very much

    lua.set_function("getGameObject", [&]()->GameObject* {
        return getGameObject();
    });

    lua.set_function("getAnimator", [&](GameObject* _gameObj)->Animator* {
        return _gameObj->getComponent<Animator>().get();
    });
    lua.set_function("getTransform", [&](GameObject* _gameObj) -> Transform* {
        return _gameObj->getComponent<Transform>().get();
    });

    lua.set_function("setTarget", [&](GameObject* _target) -> void {
        return setTarget(_target);
    });

    lua.set_function("inCircle", [&](glm::vec2 point, glm::vec2 center, float radius) -> bool {
        return inCircle(point, center, radius);
    });

    lua["variables"] = lua.create_table_with(
            "range", range
            );

    actions[TB_TARGETING] = "targeting";
    actions[TB_RELOADING] = "reloading";
    actions[TB_AIMING] = "aiming";
    actions[TB_SHOOTING] = "shooting";

    loadScript(actions[TB_TARGETING], R"(.\scripts\targeting.lua)", true);
//    loadScript(actions[TB_AIMING], "", true);
//    loadScript(actions[TB_SHOOTING], "", true);
//    loadScript(actions[TB_RELOADING], "", true);
}

void TowerBehaviourComponent::update() {

    //first of all, tower reloads if needed
    if (!readyToShoot)
//        run(actions[TB_RELOADING]);
        std::cout << actions[TB_RELOADING] << std::endl;
    // if tower doesn't have a live target, get a new one
    else if(target == nullptr) {
        std::cout << "targeting... " << scripts.find("targeting")->second->function.valid() << std::endl;
        run(actions[TB_TARGETING], gameManager->getEnemies()); //
    }
    // if the tower is ready to shoot and has a target, but hasn't aimed yet, then calc where to shoot
    else if (aimPos == glm::vec3(-1))
        std::cout << actions[TB_AIMING] << std::endl;
        //run(actions[TB_AIMING]);
    // if tower is ready and has calculated where to shoot, then shoot away!
    else if (aimPos != glm::vec3(-1) && readyToShoot)
        std::cout << actions[TB_SHOOTING] << std::endl;
    //run(actions[TB_SHOOTING]);
    if(target)
        std::cout << std::endl << "Targeting skull with enemy id " << target->getComponent<EnemyComponent>()->getEnemyNumber() << " and set number " << target->getComponent<EnemyComponent>()->getEnemySetNumber() << std::endl;
}

float TowerBehaviourComponent::getRange() const {
    return range;
}

void TowerBehaviourComponent::setRange(float _range) {
    TowerBehaviourComponent::range = _range;
}

bool TowerBehaviourComponent::isReadyToShoot() const {
    return readyToShoot;
}

void TowerBehaviourComponent::setReadyToShoot(bool _readyToShoot) {
    TowerBehaviourComponent::readyToShoot = _readyToShoot;
}

float TowerBehaviourComponent::getProjectileSpeed() const {
    return projectileSpeed;
}

void TowerBehaviourComponent::setProjectileSpeed(float _projectileSpeed) {
    TowerBehaviourComponent::projectileSpeed = _projectileSpeed;
}

float TowerBehaviourComponent::getReloadSpeed() const {
    return reloadSpeed;
}

void TowerBehaviourComponent::setReloadSpeed(float _reloadSpeed) {
    TowerBehaviourComponent::reloadSpeed = _reloadSpeed;
}

GameObject *TowerBehaviourComponent::getTarget() const {
    return target;
}

void TowerBehaviourComponent::setTarget(GameObject *_target) {
    TowerBehaviourComponent::target = _target;
}

const glm::vec3 &TowerBehaviourComponent::getAimPos() const {
    return aimPos;
}

void TowerBehaviourComponent::setAimPos(const glm::vec3 &_aimPos) {
    TowerBehaviourComponent::aimPos = _aimPos;
}

bool TowerBehaviourComponent::targetInRange() const {
    if (!target)
        return false;
    auto targetPos = target->getComponent<Transform>()->globalPosition();
    auto turretPos = gameObject->getComponent<Transform>()->globalPosition();
    return inCircle(glm::vec2(targetPos.x, targetPos.z), glm::vec2(turretPos.x, turretPos.z), range);
}

// helper method to check whether a point is inside a circle of a given radius (using X and Z coordinates)
bool TowerBehaviourComponent::inCircle(glm::vec2 point, glm::vec2 center, float radius) {
    // optimised to first check whether point is clearly out of bounds, and eventually perform expensive calculations
    // as needed
    float dX = abs(point.x - center.x);
    if (dX > radius)
        return false;
    float dY = abs(point.y - center.y);
    if (dY > radius)
        return false;
    return pow(dX, 2) + pow(dY, 2) < pow(radius, 2);
}
