//
// Created by Morten Nobel Jørgensen on 2018-11-10.
//

#include "../Component.hpp"
#include "btBulletDynamicsCommon.h"
#include "glm/glm.hpp"

// #include "BulletCollision/CollisionDispatch/btGhostObject.h"


class TransformComponent;
class btGhostObject;
class btPairCachingGhostObject;

class GhostObject : public Component {
public:
    explicit GhostObject(GameObject* gameObject);
    ~GhostObject();

    void initGhostObjectWithSphere(float radius);

    btGhostObject* getGhostObject();

private:
    int collisionStartFrameId;
    int collisionStartUpdateFrameId;
    int collisionStartEndFrameId;
    void updateTransformFromPhysicsWorld();


    // btPairCachingGhostObject* ghostObject = nullptr;
    btGhostObject* ghostObject = nullptr;
    TransformComponent* transform;

    btCollisionShape* shape = nullptr;


    friend class BulletPhysics;
    friend class Scene;
    friend class LevelScene;
};


