//
// Created by kb on 27/01/2021.
//

#include <LinearMath/btVector3.h>
#include "TowerBehaviourComponent.hpp"
#include "Transform.hpp"
#include "Animator.hpp"
#include "EnemyComponent.hpp"
#include "Pathfinder.hpp"
#include "collisions/ProjectileCollisionHandler.hpp"
#include "lifespans/ProjectileLifespanComponent.hpp"
#include "physics/RigidBody.hpp"

TowerBehaviourComponent::TowerBehaviourComponent(GameObject* gameObject)
        : Component(gameObject) {
    auto transform = gameObject->getComponent<Transform>();
//    glm::vec3 offset = {0*transform->scale.x, 1.2*transform->scale.y, 2*transform->scale.z};
//    projectileStart = transform->globalPosition() + offset;
    lua.set_function("getGameObject", [&]()->GameObject* {
        return getGameObject();
    });

//    auto btVector_type = lua.new_usertype<btVector3>("btVector3",
//                                                     sol::constructors<btVector3(float, float, float)>(),
//                                                     sol::meta_function::multiplication, sol::resolve
//                                                     );
    //Incomplete implementation of Animator
    auto animator_type = lua.new_usertype<Animator>( "Animator",
                                "setAnimationState", &Animator::setAnimationState,
                                "getAnimationState", &Animator::getAnimationState,
                                "getAnimationForState", &Animator::getAnimationForState
    );

    auto animation_type = lua.new_usertype<Animation>( "Animation",
                                                     "hasEnded", &Animation::hasEnded
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

    lua.set_function("getProjectileAirTime", [&]() -> float {
        return getProjectileAirTime();
    });

    lua.set_function("getLaunchTime", [&]() -> float {
        return getLaunchTime();
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

//    lua.set_function("setVelocity", [](GameObject* object, btVector3 velocity) {
//        object->getComponent<RigidBody>()->getRigidBody()->setLinearVelocity(velocity);
//    });

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

void TowerBehaviourComponent::update(float deltaTime) {
    bool hasTargetInRange = targetInRange();
    if(!hasTargetInRange) // if target leaves range
        target = nullptr;

    //first of all, tower reloads if needed
    if (!readyToShoot)
        auto kek = 2;
//        run(actions[TB_RELOADING]);
//        std::cout << actions[TB_RELOADING] << std::endl;
    // if tower doesn't have a live target in range, get a new one
    if(!target) {
        run(actions[TB_TARGETING], gameObject->getScene()->getEnemies()); //
    }
    // if the tower is ready to shoot and has a target, but hasn't aimed yet, then calc where to shoot
    if (hasTargetInRange && aimPos == glm::vec3(-1)) {
        std::cout << actions[TB_AIMING] << std::endl;
        run(actions[TB_AIMING], target->getComponent<EnemyComponent>());
    }
    // if tower is ready and has calculated where to shoot, then shoot away!
    if (aimPos != glm::vec3(-1) && readyToShoot) {
//        shoot(deltaTime);
    }
//
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

float TowerBehaviourComponent::getProjectileAirTime() const {
    return projectileAirTime;
}

void TowerBehaviourComponent::setProjectileAirTime(float projectileAirTime_) {
    TowerBehaviourComponent::projectileAirTime = projectileAirTime_;
}

float TowerBehaviourComponent::getReloadTime() const {
    return reloadTime;
}

void TowerBehaviourComponent::setReloadTime(float reloadTime_) {
    TowerBehaviourComponent::reloadTime = reloadTime_;
}

const glm::vec3 &TowerBehaviourComponent::getAimPos() const {
    return aimPos;
}

void TowerBehaviourComponent::setAimPos(const glm::vec3 &aimPos_) {
    TowerBehaviourComponent::aimPos = aimPos_;
}

bool TowerBehaviourComponent::targetInRange() const {
    if (!target || target->isQueuedForDeletion())
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

void TowerBehaviourComponent::shoot(float deltaTime) {
    if(!readyToShoot)
        return;
    auto arm = gameObject->getChildByName("Arm");
    if(arm) {
        auto armAN = arm->getComponent<Animator>();
        if(armAN->getAnimationState() != "launch")
            armAN->setAnimationState("launch");
    }
    auto transform = gameObject->getComponent<Transform>();
    transform->lookAt(glm::vec3{aimPos.x, transform->globalPosition().y, aimPos.z}, glm::vec3(0,1,0));
    makeProjectile();
    std::cout << "making dat projectile baby" << std::endl;
    setReadyToShoot(false);
//    auto arm = gameObject->getChildByName("Arm");
//    if(!arm) {
//        std::cout << "no arm!" << std::endl;
//        return;
//    }
//    auto animator = arm->getComponent<Animator>();
//    if(animator) {
//        if(animator->getAnimationState() != "launch")
//            animator->setAnimationState("launch");
//        else if(!animator || animator->getAnimationForState("launch")->hasEnded(deltaTime)) {
//            auto direction = glm::normalize(aimPos - projectileStart);
//            auto distance = fabs(glm::length(aimPos - projectileStart));
//            arrow->getComponent<Transform>()->lookAt(aimPos, glm::vec3(0, 1, 0));
//
//            auto arrowRigidBody = arrow->getComponent<RigidBody>()->getRigidBody();
//            btVector3 arrowSpeed = {direction.x, direction.y, direction.z};
//            arrowSpeed *= distance/projectileAirTime; // get required speed
//            arrowRigidBody->setLinearVelocity(arrowSpeed);
//            readyToShoot = false;
//            aimPos = glm::vec3(-1);
//        }
//    }
}

float TowerBehaviourComponent::getLaunchTime() const {
    return launchTime;
}

void TowerBehaviourComponent::setLaunchTime(float launchTime) {
    TowerBehaviourComponent::launchTime = launchTime;
}

std::shared_ptr<GameObject> TowerBehaviourComponent::makeProjectile() {
    auto projectile_ = gameObject->getScene()->createGameObject(gameObject->getName() + "TowerProjectile");
    projectile_->setParent(gameObject);
    auto projectileTR = projectile_->getComponent<Transform>();
    projectileTR->position = projectile.position;
    projectileTR->scale = projectile.scale;
    projectileTR->rotation = projectile.rotation;
    auto projectileMR = projectile_->addComponent<ModelRenderer>();
    projectileMR->setModel(Model::create().withOBJ(projectile.model).build());
    auto projectileRB = projectile_->addComponent<RigidBody>();
    if (projectile.hitboxType == "box")
        projectileRB->initRigidBodyWithBox(projectile.hitboxSize, projectile.mass, PROJECTILES, ENEMIES);
    else if (projectile.hitboxType == "sphere") {
        std::cout << projectile.radius << std::endl;
        projectileRB->initRigidBodyWithSphere(projectile.radius, projectile.mass, PROJECTILES, ENEMIES);
    }
    projectileRB->getRigidBody()->setAngularFactor({0, 0, 0});
    projectileRB->getRigidBody()->setGravity({0, 0, 0});
    projectileTR->setModelRenderer(projectileMR);
    auto projectileCH = projectile_->addComponent<ProjectileCollisionHandler>();
    projectileCH->setDamage(projectile.damage);
    projectile_->addComponent<ProjectileLifespanComponent>();

    return projectile_;
}

const TowerProjectile &TowerBehaviourComponent::getProjectile() const {
    return projectile;
}

void TowerBehaviourComponent::setProjectile(const TowerProjectile &projectile) {
    TowerBehaviourComponent::projectile = projectile;
}
