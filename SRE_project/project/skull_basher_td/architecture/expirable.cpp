
#include "Expirable.hpp"
#include "Transform.hpp"
#include "scenes/Scene.hpp"
#include <cmath>

#include "./physics/RigidBody.hpp"
#include "./sound/SourceManager.hpp"
#include "../GameManager.hpp"
#include "glm/gtx/rotate_vector.hpp"

using namespace glm;
Expirable::Expirable(GameObject* gameObject)
 : Component(gameObject)
{
    start_life = std::chrono::steady_clock::now();
}

Expirable::~Expirable()
{

}


void Expirable::update(float deltaTime){

    std::chrono::steady_clock::time_point time_now = std::chrono::steady_clock::now();
    int time_elapsed_milli = std::chrono::duration_cast<std::chrono::milliseconds>(time_now - start_life).count();

    if (time_elapsed_milli > lifespan_millisec)
        gameObject->deleteMe = true; 

}

