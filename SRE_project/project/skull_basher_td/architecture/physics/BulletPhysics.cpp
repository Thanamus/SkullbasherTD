//
// Created by Morten Nobel Jørgensen on 2018-11-08.
//

#include "BulletPhysics.hpp"
#include "RigidBody.hpp"
#include "GhostObject.hpp"
#include "../GameObject.hpp"
#include "../scenes/Scene.hpp"

#include "BulletCollision/CollisionDispatch/btGhostObject.h"

btGhostPairCallback* m_ghostPairCallback = NULL;				// Needed once to enable ghost objects inside Bullet
btGhostObject* m_ghostObject = NULL;							// simple aabb ghost object (keeps track of the objects whose aabbs intersect its own collision shape aabb: this is called "broadphase stage collision detection")
btPairCachingGhostObject* m_pairCachingGhostObject=NULL;
// "Portable" method; prerequisite call: m_dynamicsWorld->getBroadphase()->getOverlappingPairCache()->setInternalGhostPairCallback(m_ghostPairCallback); 
static void GetCollidingObjectsInsidePairCachingGhostObject(btDiscreteDynamicsWorld* m_dynamicsWorld,btPairCachingGhostObject* m_pairCachingGhostObject,btAlignedObjectArray < btCollisionObject* >& collisionArrayOut);
static void motorPreTickCallback (btDynamicsWorld *world, btScalar timeStep);

struct CollisionId {
    size_t collisionId;
    GameObject* bodyA;
    GameObject* bodyB;

    CollisionId(size_t collisionId, GameObject *bodyA, GameObject *bodyB) : collisionId(collisionId), bodyA(bodyA),
                                                                            bodyB(bodyB) {}
};

namespace {
 Scene* scene;

 bool sceneContains(GameObject* ptr){
     for (auto go : scene->getGameObjects()){
         if (go.get() == ptr){
             return true;
         }
     }
     return false;
 }

// based on https://github.com/kripken/ammo.js/issues/82#issuecomment-426429129
bool contactUpdatedCallback(btManifoldPoint& cp,void* body0,void* body1){
    bool collisionBegin = cp.m_userPersistentData == nullptr;
    bool body0isGhost = false;
    bool body1isGhost = false;
    auto btRigidBody0 = static_cast<btRigidBody *>(body0);
    auto btRigidBody1 = static_cast<btRigidBody *>(body1);
    auto rigidBody0 = static_cast<RigidBody *>(btRigidBody0->getUserPointer());
    auto rigidBody1 = static_cast<RigidBody *>(btRigidBody1->getUserPointer());

    // check for ghost object collision
    // program crashes from ghost object
    // auto ghostCheck0 = static_cast<btGhostObject * >(body0);
    // auto ghostCheck1 = static_cast<GhostObject * >(body1);
    // auto ghostCheck0 = static_cast<GhostObject * >(body0);
    // auto ghostCheck1 = static_cast<GhostObject * >(body1);
    // auto ghostCheck0 = rigidBody0->getGameObject()->getComponent<GhostObject>();
    // auto ghostCheck1 = rigidBody0->getGameObject()->getComponent<GhostObject>();
    // auto btGhostBody0 = static_cast<btGhostObject *>(body0);
    // auto btGhostBody1 = static_cast<btGhostObject *>(body1);

    // auto ghostCheck0 = static_cast<GhostObject * >(btGhostBody0->getUserPointer());
    // auto ghostCheck1 = static_cast<GhostObject * >(btGhostBody0->getUserPointer());

    if (rigidBody0->getGameObject()->getComponent<GhostObject>() != nullptr)
    {
        // std::cout << "Ghost object found on rigid body 0" << std::endl;
        body0isGhost = true;
    }
    if (rigidBody1->getGameObject()->getComponent<GhostObject>() != nullptr)
    {
        // std::cout << "Ghost object found on rigid body 1" << std::endl;
        body1isGhost = true;
    }
    
    

    // if(ghostCheck0 != nullptr) body0isGhost = true;
    // if(ghostCheck1 != nullptr) body1isGhost = true;
    // if(ghostCheck0->getGhostObject() != nullptr) body0isGhost = true;
    // if(ghostCheck0->getGhostObject() != nullptr) body1isGhost = true;

    // if (ghostCheck1 != nullptr || ghostCheck2 != nullptr)
    // {
    //     std::cout << "one body was ghostObject";
    // }
    // ------- from: https://pybullet.org/Bullet/phpBB3/viewtopic.php?t=7468
    // Prepare for getting all the contact manifolds for one Overlapping Pair
    // btManifoldArray manifoldArray;
    // // Get all the Overlapping Pair
    // btBroadphasePairArray &pairArray = ghostObject->getOverlappingPairCache()->getOverlappingPairArray();
    // int numPairs = pairArray.size();

    // for (int i = 0; i < numPairs; i++)
    // {
    //     manifoldArray.clear();

    //     const btBroadphasePair &pair = pairArray[i];

    //     //unless we manually perform collision detection on this pair, the contacts are in the dynamics world paircache:
    //     //The next line fetches the collision information for this Pair
    //     btBroadphasePair *collisionPair = dynamicsWorld->getPairCache()->findPair(pair.m_pProxy0, pair.m_pProxy1);
    //     if (!collisionPair)
    //         continue;

    //     // Read out the all contact manifolds for this Overlapping Pair
    //     if (collisionPair->m_algorithm)
    //         collisionPair->m_algorithm->getAllContactManifolds(manifoldArray);

    //     for (int j = 0; j < manifoldArray.size(); j++)
    //     {
    //         btPersistentManifold *manifold = manifoldArray[j];

    //         // Check if the first object in the Pair is GhostObject or not.
    //         btScalar directionSign = manifold->getBody0() == m_ghostObject ? btScalar(-1.0) : btScalar(1.0);
    //         for (int p = 0; p < manifold->getNumContacts(); p++)
    //         {
    //             const btManifoldPoint &pt = manifold->getContactPoint(p);
    //             if (pt.getDistance() < 0.f)
    //             {
    //                 // Actually you can get the local information from this Overlapping pair, not just world Position
    //                 const btVector3 &ptA = pt.getPositionWorldOnA();
    //                 const btVector3 &ptB = pt.getPositionWorldOnB();
    //                 const btVector3 &normalOnB = pt.m_normalWorldOnB;
    //                 /// work here
    //             }
    //         }
    //     }
    // }
    //--------- end from:
    // TODO: check if these can be made shared pointers
    // GameObject* body0GameObject;
    // GameObject* body1GameObject;

    if (collisionBegin){
        static size_t collisionId = 0;
        collisionId++;
        auto name0 = rigidBody0->getGameObject()->getName();
        std::cout << "colision on: " << name0 << std::endl;
        auto name1 = rigidBody1->getGameObject()->getName();
        std::cout << "colision on: " << name1 << std::endl;

        // std::cout << "attempting to check collision" << std::endl;

        // if (body0isGhost)
        // {
        //     // if collision is with a ghost, get the gameObject from the ghost
        //     // body0GameObject = ghostCheck0->getGameObject();
        // } else {
        //     body0GameObject = rigidBody0->getGameObject();
        // }
        // if (body1isGhost)
        // {
        //     // if collision is with a ghost, get the gameObject from the ghost
        //     // body1GameObject = ghostCheck1->getGameObject();
        // } else {
        //     body1GameObject = rigidBody1->getGameObject();
        // }
        // body0GameObject = rigidBody0->getGameObject();
        // body1GameObject = rigidBody1->getGameObject();

        // cp.m_userPersistentData = new CollisionId(collisionId, body0GameObject, body1GameObject); //original
        cp.m_userPersistentData = new CollisionId(collisionId, rigidBody0->getGameObject(), rigidBody1->getGameObject()); //original
    }
    CollisionId* id = (CollisionId*)cp.m_userPersistentData;
    glm::vec3 pointOnA (cp.getPositionWorldOnA().x(), cp.getPositionWorldOnA().y(), cp.getPositionWorldOnA().z());
    glm::vec3 pointOnB (cp.getPositionWorldOnB().x(), cp.getPositionWorldOnB().y(), cp.getPositionWorldOnB().z());
    // for (auto ph : body0GameObject->getCollisionHandlers()){
    for (auto ph : rigidBody0->getGameObject()->getCollisionHandlers()){
        // ph->onCollision(id->collisionId, body1GameObject, pointOnA, collisionBegin); // original
        ph->onCollision(id->collisionId, rigidBody1->getGameObject(), pointOnA, collisionBegin);
    }
    for (auto ph : rigidBody1->getGameObject()->getCollisionHandlers()){
    // for (auto ph : body1GameObject->getCollisionHandlers()){
        ph->onCollision(id->collisionId, rigidBody0->getGameObject(), pointOnB, collisionBegin);
        // ph->onCollision(id->collisionId, body0GameObject, pointOnB, collisionBegin);
    }
    // original
    // for (auto ph : rigidBody0->getGameObject()->getCollisionHandlers()){
    //     ph->onCollision(id->collisionId, rigidBody1, pointOnA, collisionBegin); // original
    //     // ph->onCollision(id->collisionId, body0GameObject, pointOnA, collisionBegin);
    // }
    // for (auto ph : rigidBody1->getGameObject()->getCollisionHandlers()){
    //     ph->onCollision(id->collisionId, rigidBody0, pointOnB, collisionBegin);
    // }
    return true;
}

bool contactDestroyedCallback(void * data) {
    CollisionId* id = (CollisionId*)data;
    if (sceneContains(id->bodyA)){
        for (auto ph : id->bodyA->getCollisionHandlers()){
            ph->onCollisionEnd(id->collisionId);
        }
    }
    if (sceneContains(id->bodyB)){
        for (auto ph : id->bodyB->getCollisionHandlers()){
            ph->onCollisionEnd(id->collisionId);
        }
    }
    delete id;
    return true;
}

// Performs raycasting on the world and returns the point of collision
}

BulletPhysics::BulletPhysics() {
    gContactProcessedCallback = contactUpdatedCallback;
    gContactDestroyedCallback = contactDestroyedCallback;
    // gContactDestroyedCallback = contactDestroyedCallback;

    // gMotorPreTickCallback = motorPreTickCallback;

    broadphase = new btDbvtBroadphase();
    collisionConfiguration = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collisionConfiguration);
    solver = new btSequentialImpulseConstraintSolver();
    world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
    world->setDebugDrawer(&debugDrawObj);

    setGravity(gravity);

    // from: https://pybullet.org/Bullet/phpBB3/viewtopic.php?t=6195
    m_ghostPairCallback = new btGhostPairCallback(); 
	world->getBroadphase()->getOverlappingPairCache()->setInternalGhostPairCallback(m_ghostPairCallback);	
    // based on https://pybullet.org/Bullet/phpBB3/viewtopic.php?t=7468
    // world->setInternalTickCallback(motorPreTickCallback,this,true);

}


// void motorPreTickCallback (btDynamicsWorld *world, btScalar timeStep)
// {
//     // if(world){

//     //     auto thing = world->getPairCache();
//     // }

//     // from: https://pybullet.org/Bullet/phpBB3/viewtopic.php?t=6195
//     if (world)
// 	{
// 		btAlignedObjectArray < btCollisionObject* > objsInsidePairCachingGhostObject;	// We might want this to be a member variable...							
// 		btAlignedObjectArray < btCollisionObject* >* pObjsInsideGhostObject = NULL;		// We will store a reference of the current array in this pointer
// 		const btAlignedObjectArray < btCollisionObject* >& objs = world->getCollisionObjectArray();
		
//         for (int i=0,sz=objs.size();i<sz;i++)	{
// 			btCollisionObject* o = objs[i];
// 			btGhostObject* go = btGhostObject::upcast(o);

// 			if (go)	{
//                 // go - Ghost Object is a ghost object
// 				objsInsidePairCachingGhostObject.resize(0);
// 				btPairCachingGhostObject* pgo = dynamic_cast < btPairCachingGhostObject* > (go);	// No upcast functionality...
// 				if (pgo)	{
// 					GetCollidingObjectsInsidePairCachingGhostObject(static_cast < btDiscreteDynamicsWorld* > (world),pgo,objsInsidePairCachingGhostObject);
// 					pObjsInsideGhostObject = &objsInsidePairCachingGhostObject;

                    
//                     // // for (int i = 0; i < pObjsInsideGhostObject->getNumOverlappingObjects(); i++)
//                     // for (int i = 0; i < pObjsInsideGhostObject->size(); i++)
//                     // {
//                     //     // btRigidBody *pRigidBody = dynamic_cast<btRigidBody *>(ghostObject->getOverlappingObject(i));
                        
//                     //     auto rigidBody0 = static_cast<RigidBody *>(pObjsInsideGhostObject[i]->getUserPointer());
//                     //     // auto rigidBody1 = static_cast<RigidBody *>(btRigidBody1->getUserPointer());
//                     //     // do whatever you want to do with these pairs of colliding objects
//                     //     std::cout << "ghost object triggered";
//                     // }
//                     // std::cout << "inside pgo" << std::endl;
// 				}
// 				else {
// 					pObjsInsideGhostObject = &go->getOverlappingPairs();	// It's better not to try and copy the whole array, but to keep a reference to it!
// 					// Side Note: btAlignedObjectArray < btCollisionObject* > objs = go->getOverlappingPairs(); (at the moment) makes my program crash on my system...
// 					// Nevermind, that was the wrong way of doing it: btAlignedObjectArray < btCollisionObject* >& objs = go->getOverlappingPairs(); is much better.
// 				    // std::cout << "getting overlapping pairs" << std::endl;
//                 }	
// 				// Here pObjsInsideGhostObject should be valid.
				
// 				// ProcessObectsInsideGhostObjects(*pObjsInsideGhostObject,pgo);
// 			}
		
// 		}	
// 	}

//   	// for(int i = 0; i < ghostObject->getNumOverlappingObjects(); i++)
//  	// {
//     //             btRigidBody *pRigidBody = dynamic_cast<btRigidBody *>(ghostObject->getOverlappingObject(i));
//     //            // do whatever you want to do with these pairs of colliding objects
//     //            std::cout << "ghost object triggered";
//     //  }
// }

// // Portable static method: prerequisite call: m_dynamicsWorld->getBroadphase()->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback()); 
// void GetCollidingObjectsInsidePairCachingGhostObject(btDiscreteDynamicsWorld* m_dynamicsWorld,btPairCachingGhostObject* m_pairCachingGhostObject,btAlignedObjectArray < btCollisionObject* >& collisionArrayOut)	{
// 	collisionArrayOut.resize(0);
// 	if (!m_pairCachingGhostObject || !m_dynamicsWorld) return;
// 	const bool addOnlyObjectsWithNegativeDistance(true);	// With "false" things don't change much, and the code is a bit faster and cleaner...

// 	//#define USE_PLAIN_COLLISION_WORLD // We dispatch all collision pairs of the ghost object every step (slow)
// 	#ifdef USE_PLAIN_COLLISION_WORLD
// 	//======================================================================================================
// 	// I thought this line was no longer needed, but it seems to be necessary (and I believe it's an expensive call):
// 	m_dynamicsWorld->getDispatcher()->dispatchAllCollisionPairs(m_pairCachingGhostObject->getOverlappingPairCache(), m_dynamicsWorld->getDispatchInfo(), m_dynamicsWorld->getDispatcher());
// 	// Maybe the call can be automatically triggered by some other Bullet call (I'm almost sure I could comment it out in another demo I made long ago...)
// 	// So by now the general rule is: in real projects, simply comment it out and see if it works!
// 	//======================================================================================================
// 	// UPDATE: in dynamic worlds, the line above can be commented out and the broadphase pair can be retrieved through the call to findPair(...) below.
// 	// In collision worlds probably the above line is needed only if collision detection for all the bodies hasn't been made... This is something
// 	// I'm still not sure of... the general rule is to try to comment out the line above and try to use findPair(...) and see if it works whenever possible....
// 	//======================================================================================================
// 	#endif //USE_PLAIN_COLLISION_WORLD
	
// 	btBroadphasePairArray& collisionPairs = m_pairCachingGhostObject->getOverlappingPairCache()->getOverlappingPairArray();
//   	const int	numObjects=collisionPairs.size();	
// 	static btManifoldArray	m_manifoldArray;
// 	bool added;
// 	for(int i=0;i<numObjects;i++)	{
//     	m_manifoldArray.resize(0);
    	
// 		#ifdef USE_PLAIN_COLLISION_WORLD
// 		const btBroadphasePair& collisionPair = collisionPairs[i];
// 		if (collisionPair.m_algorithm) collisionPair.m_algorithm->getAllContactManifolds(m_manifoldArray);
// 		else {	// THIS SHOULD NEVER HAPPEN, AND IF IT DOES, PLEASE RE-ENABLE the "call" a few lines above...
// 			printf("No collisionPair.m_algorithm - probably m_dynamicsWorld->getDispatcher()->dispatchAllCollisionPairs(...) must be missing.\n");	
// 		}	
// 		#else // USE_PLAIN_COLLISION_WORLD	
// 		const btBroadphasePair& cPair = collisionPairs[i];
// 		//unless we manually perform collision detection on this pair, the contacts are in the dynamics world paircache:
//     	const btBroadphasePair* collisionPair = m_dynamicsWorld->getPairCache()->findPair(cPair.m_pProxy0,cPair.m_pProxy1);
//     	if (!collisionPair) continue;		
// 		if (collisionPair->m_algorithm) collisionPair->m_algorithm->getAllContactManifolds(m_manifoldArray);
// 		else {	// THIS SHOULD NEVER HAPPEN, AND IF IT DOES, PLEASE RE-ENABLE the "call" a few lines above...
// 			printf("No collisionPair.m_algorithm - probably m_dynamicsWorld->getDispatcher()->dispatchAllCollisionPairs(...) must be missing.\n");	
// 		}
// 		#endif //USE_PLAIN_COLLISION_WORLD
		
// 		added = false;
// 		for (int j=0;j<m_manifoldArray.size();j++)	{
// 			btPersistentManifold* manifold = m_manifoldArray[j];
// 			// Here we are in the narrowphase, but can happen that manifold->getNumContacts()==0:
// 			if (addOnlyObjectsWithNegativeDistance)	{
// 				for (int p=0,numContacts=manifold->getNumContacts();p<numContacts;p++)			{
// 					const btManifoldPoint&pt = manifold->getContactPoint(p);
// 					if (pt.getDistance() < 0.0) 	{
// 						// How can I be sure that the colObjs are all distinct ? I use the "added" flag.
// 						collisionArrayOut.push_back((btCollisionObject*) (manifold->getBody0() == m_pairCachingGhostObject ? manifold->getBody1() : manifold->getBody0()));
// 						added = true;
// 						break;
// 					}	
// 				}
// 			if (added) break;
// 			}
// 			else if (manifold->getNumContacts()>0) {
// 				collisionArrayOut.push_back((btCollisionObject*) (manifold->getBody0() == m_pairCachingGhostObject ? manifold->getBody1() : manifold->getBody0()));
// 				break;
// 			}
// 		}
// 	}	
// }


BulletPhysics::~BulletPhysics() {
    delete world;
    delete solver;
    delete collisionConfiguration;
    delete dispatcher;
    delete broadphase;
}

const glm::vec3 &BulletPhysics::getGravity() const {
    return gravity;
}

void BulletPhysics::setGravity(const glm::vec3 &gravity) {
    BulletPhysics::gravity = gravity;
    world->setGravity(btVector3(gravity.x, gravity.y, gravity.z));
}

void BulletPhysics::step(Scene* scene_) {
    scene = scene_;
    world->stepSimulation(timeStep, maxSubSteps);

}

void BulletPhysics::debugDrawNewFrame() {
    debugDrawObj.newFrame();
    world->debugDrawWorld();
}

void BulletPhysics::debugDraw(sre::RenderPass &renderPass) {
    debugDrawObj.render(renderPass);
}

bool BulletPhysics::RaycastWorld(const glm::vec3 &Start, glm::vec3 &End) {

    btCollisionWorld::ClosestRayResultCallback RayCallback(btVector3(Start.x, Start.y, Start.z), btVector3(End.x, End.y, End.z));
    //RayCallback.m_collisionFilterMask = came;

    // Perform raycast
    world->rayTest(btVector3(Start.x, Start.y, Start.z), btVector3(End.x, End.y, End.z), RayCallback);
    if(RayCallback.hasHit()) {

        btVector3 target = RayCallback.m_hitPointWorld;
        btVector3 Normal = RayCallback.m_hitNormalWorld;
        return true;
    }

    return false;
}
