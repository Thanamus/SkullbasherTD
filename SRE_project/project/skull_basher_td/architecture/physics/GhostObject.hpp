//
// Created by Morten Nobel Jørgensen on 2018-11-10.
//

#include "../Component.hpp"
#include "btBulletDynamicsCommon.h"
#include "glm/glm.hpp"

// #include "BulletCollision/CollisionDispatch/btGhostObject.h"


class Transform;
class btGhostObject;
class btPairCachingGhostObject;

class GhostObject : public Component {
public:
    explicit GhostObject(GameObject* gameObject);
    ~GhostObject() final;
    // void initRigidBodyWithSphere(float radius, float mass = 0);
    void initGhostObjectWithSphere(float radius);

    // void initRigidBodyWithBox(glm::vec3 halfExtend, float mass = 1.0);
    // void initRigidBodyWithStaticPlane(glm::vec3 planeNormal, float planeDist);
    // void initRigidBody(btRigidBody::btRigidBodyConstructionInfo info);
    // btRigidBody* getRigidBody();
    btGhostObject* getGhostObject();

    // void setLinearVelocityOnRigidBody(btVector3 linear_velocity);
    // void motorPreTickCallback (btDynamicsWorld *world, btScalar timeStep);
private:
    int collisionStartFrameId;
    int collisionStartUpdateFrameId;
    int collisionStartEndFrameId;
    void updateTransformFromPhysicsWorld();
    // btRigidBody* rigidBody = nullptr;

    // btPairCachingGhostObject* ghostObject = nullptr;
    btGhostObject* ghostObject = nullptr;
    Transform* transform;

    btCollisionShape* shape = nullptr;
    // btDefaultMotionState* fallMotionState = nullptr;

    friend class BulletPhysics;
    friend class Scene;
    friend class LevelScene;
};


