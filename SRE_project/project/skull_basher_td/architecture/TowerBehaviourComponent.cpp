//
// Created by kb on 27/01/2021.
//

#include <LinearMath/btVector3.h>
#include "TowerBehaviourComponent.hpp"
#include "TransformComponent.hpp"
#include "AnimatorComponent.hpp"
#include "EnemyComponent.hpp"
#include "Pathfinder.hpp"
#include "collisions/ProjectileCollisionHandlerComponent.hpp"
#include "lifespans/ProjectileLifespanComponent.hpp"
#include "physics/RigidBodyComponent.hpp"

TowerBehaviourComponent::TowerBehaviourComponent(GameObject* gameObject)
        : Component(gameObject) {
    auto transform = gameObject->getComponent<TransformComponent>();
//    glm::vec3 offset = {0*transform->scale.x, 1.2*transform->scale.y, 2*transform->scale.z};
//    projectileStart = transform->globalPosition() + offset;
    lua.set_function("getGameObject", [&]()->GameObject* {
        return getGameObject();
    });

    auto btVector_type = lua.new_usertype<btVector3>("btVector3",
                                                     sol::constructors<btVector3(float, float, float)>(),
                                                     sol::meta_function::multiplication, sol::overload(
                                                             sol::resolve<btVector3&(const btScalar&)>(&btVector3::operator*=),
                                                             sol::resolve<btVector3&(const btVector3&)>(&btVector3::operator*=)
                                                             )
                                                     );
    //Incomplete implementation of AnimatorComponent
    auto animator_type = lua.new_usertype<AnimatorComponent>("AnimatorComponent",
                                                             "setAnimationState", &AnimatorComponent::setAnimationState,
                                                             "getAnimationState", &AnimatorComponent::getAnimationState,
                                                             "getAnimationForState", &AnimatorComponent::getAnimationForState
    );

    auto modelRenderer_type = lua.new_usertype<ModelRendererComponent>("ModelRendererComponent",
                                                                       "active", &ModelRendererComponent::active
                                                              );

    auto animation_type = lua.new_usertype<Animation>( "Animation",
                                                     "hasEnded", &Animation::hasEnded
    );

    //Incomplete implementation of Transform
    auto transform_type = lua.new_usertype<TransformComponent>( "Transform",
                                 "globalTransform", &TransformComponent::globalTransform,
                                 "lookAt", sol::overload(
                                         static_cast<void (TransformComponent::*)(glm::vec3, glm::vec3)>(&TransformComponent::lookAt),
                                         static_cast<void (TransformComponent::*)(TransformComponent*, glm::vec3)>(&TransformComponent::lookAt)
                                         ),
                                 "globalPosition", &TransformComponent::globalPosition
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

    lua.set_function("getAnimator", [&](GameObject* _gameObj)->AnimatorComponent* {
        return _gameObj->getComponent<AnimatorComponent>().get();
    });

    lua.set_function("getTransform", [&](GameObject* _gameObj) -> TransformComponent* {
        return _gameObj->getComponent<TransformComponent>().get();
    });

    lua.set_function("getRigidBody", [&](GameObject* _gameObj) -> RigidBodyComponent* {
        return _gameObj->getComponent<RigidBodyComponent>().get();
    });

    lua.set_function("getModelRenderer", [&](GameObject* _gameObj) -> ModelRendererComponent* {
        return _gameObj->getComponent<ModelRendererComponent>().get();
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

    lua.set_function("getAimPos", [&]() -> glm::vec3 {
        return getAimPos();
    });


    lua.set_function("setAimPos", [&](glm::vec3 _aimPos) -> void {
        return setAimPos(_aimPos);
    });

    lua.set_function("setReadyToShoot", [&](bool ready) -> void {
        return setReadyToShoot(ready);
    });

    lua.set_function("inCircle", [&](glm::vec2 point, glm::vec2 center, float radius) -> bool {
        return inCircle(point, center, radius);
    });

    lua.set_function("makeProjectile", [&]() -> GameObject* {
        return makeProjectile();
    });

    lua.set_function("setVelocity", [&](GameObject* object, btVector3 velocity) -> void {
        object->getComponent<RigidBodyComponent>()->getRigidBody()->setLinearVelocity(velocity);
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
    loadScript(actions[TB_SHOOTING], R"(.\scripts\shooting.lua)", true);
    loadScript(actions[TB_RELOADING], R"(.\scripts\reloading.lua)", true);
}

void TowerBehaviourComponent::update(float deltaTime) {
    bool hasTargetInRange = targetInRange();
    if(!hasTargetInRange) // if target leaves range
        target = nullptr;
    else {
        auto tarPos = target->getComponent<TransformComponent>()->globalPosition();
        gameObject->getComponent<TransformComponent>()->lookAt({tarPos.x, gameObject->getComponent<TransformComponent>()->position.y, tarPos.z}, glm::vec3(0, 1, 0));
    }
    //first of all, tower reloads if needed
    if (!readyToShoot)
        run(actions[TB_RELOADING], deltaTime);
    // if tower doesn't have a live target in range, get a new one
    if(!target)
        run(actions[TB_TARGETING], gameObject->getScene()->getEnemies());
    // if the tower is ready to shoot and has a target, but hasn't aimed yet, then calc where to shoot
    if (hasTargetInRange && aimPos == glm::vec3(-1))
        run(actions[TB_AIMING], target->getComponent<EnemyComponent>());
    // if tower is ready and has calculated where to shoot, then shoot away!
    if (aimPos != glm::vec3(-1) && readyToShoot)
        run(actions[TB_SHOOTING], deltaTime);
    //
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
    auto targetPos = target->getComponent<TransformComponent>()->globalPosition();
    auto turretPos = gameObject->getComponent<TransformComponent>()->globalPosition();
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

float TowerBehaviourComponent::getLaunchTime() const {
    return launchTime;
}

void TowerBehaviourComponent::setLaunchTime(float launchTime) {
    TowerBehaviourComponent::launchTime = launchTime;
}

/*
 *  this method instantiates a GameObject acting as the tower projectile by reading from the Projectile class holding
    its information
 */
GameObject* TowerBehaviourComponent::makeProjectile() {
    // adds projectile to scene
    auto projectile_ = gameObject->getScene()->createGameObject(gameObject->getName() + " Projectile");
    auto projectileTR = projectile_->getComponent<TransformComponent>();
    // right place at the right time
    auto turretTR = gameObject->getComponent<TransformComponent>();
    projectileTR->position = projectile.position * turretTR->scale + turretTR->position;
    projectileTR->scale = projectile.scale * turretTR->scale;
    projectileTR->rotation = projectile.rotation;
    projectileTR->lookAt(aimPos, {0, 1, 0});
    auto projectileMR = projectile_->addComponent<ModelRendererComponent>();
    projectileMR->setModel(projectile.model);
    auto projectileRB = projectile_->addComponent<RigidBodyComponent>();
    if (projectile.hitboxType == "box")
        projectileRB->initRigidBodyWithBox(projectile.hitboxSize, projectile.mass, PROJECTILES, ENEMIES);
    else if (projectile.hitboxType == "sphere")
        projectileRB->initRigidBodyWithSphere(projectile.radius, projectile.mass, PROJECTILES, ENEMIES);
    auto rigidBody = projectileRB->getRigidBody();
    if (rigidBody) {
        rigidBody->setAngularFactor({0, 0, 0});
        rigidBody->setAngularVelocity({0,0,0});
        rigidBody->setGravity({0, 0, 0});
    }
    auto projectileCH = projectile_->addComponent<ProjectileCollisionHandlerComponent>();
    projectileCH->setDamage(projectile.damage);
    projectile_->addComponent<ProjectileLifespanComponent>();
    return projectile_.get();
}

const TowerProjectile &TowerBehaviourComponent::getProjectile() const {
    return projectile;
}

void TowerBehaviourComponent::setProjectile(const TowerProjectile &projectile) {
    TowerBehaviourComponent::projectile = projectile;
}
