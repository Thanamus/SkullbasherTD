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
    projectileStart = gameObject->getComponent<Transform>()->globalPosition();
    auto bounds = gameObject->getComponent<ModelRenderer>()->getMesh()->getBoundsMinMax();
    auto xOffset = abs(bounds[0].x - bounds[1].x)/2.f;
    auto zOffset = abs(bounds[0].z - bounds[1].z)/2.f;
    projectileStart = glm::vec3(projectileStart.x + xOffset, projectileStart.y + 2, projectileStart.z - zOffset);
    lua.set_function("getGameObject", [&]()->GameObject* {
        return getGameObject();
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
                                     "getPathfinder", &EnemyComponent::getPathfinder,
                                     "getPosition", &EnemyComponent::getPosition,
                                     "getGameObject", &EnemyComponent::getGameObject); // must be bound again

    auto pathfinder_type = lua.new_usertype<Pathfinder>("Pathfinder",
                                 "getCurrentPathIndex", &Pathfinder::getCurrentPathIndex,
                                 "getStartPoint", &Pathfinder::getStartPathPoint,
                                 "getNextPoint", &Pathfinder::getNextPathPoint,
                                 "getMoveSpeed", &Pathfinder::getMoveSpeed,
                                 "getDirection", &Pathfinder::getDirection,
                                 "previewPoint", &Pathfinder::previewPathPoint);

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

    lua.set_function("getTarget", [&]() -> GameObject* {
        return getTarget();
    });

    lua.set_function("targetInRange", [&]() -> bool {
        return targetInRange();
    });

    lua.set_function("setAimPos", [&](glm::vec3 _aimPos) -> void {
        return setAimPos(_aimPos);
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
    loadScript(actions[TB_AIMING], R"(.\scripts\aiming.lua)", true);
//    loadScript(actions[TB_SHOOTING], "", true);
//    loadScript(actions[TB_RELOADING], "", true);
}

void TowerBehaviourComponent::update() {
    bool hasTargetInRange = targetInRange();
    if(!hasTargetInRange) // if target leaves range
        target = nullptr;

    //first of all, tower reloads if needed
    if (!readyToShoot)
//        run(actions[TB_RELOADING]);
        std::cout << actions[TB_RELOADING] << std::endl;
    // if tower doesn't have a live target in range, get a new one
    else if(!hasTargetInRange) {
        run(actions[TB_TARGETING], gameObject->getScene()->getEnemies()); //
        if(target)
            std::cout << "Targeting skull named " << target->getName() << std::endl;
    }
    // if the tower is ready to shoot and has a target, but hasn't aimed yet, then calc where to shoot
    else if (hasTargetInRange && readyToShoot) {
        std::cout << actions[TB_AIMING] << std::endl;
        run(actions[TB_AIMING], target->getComponent<EnemyComponent>());
    }
    // if tower is ready and has calculated where to shoot, then shoot away!
    else if (aimPos != glm::vec3(-1) && readyToShoot)
        std::cout << actions[TB_SHOOTING] << std::endl;
    //run(actions[TB_SHOOTING]);
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
    std::cout << "aimpos x: " << _aimPos.x << " y: " << _aimPos.y << " z: " << _aimPos.z <<std::endl;
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
