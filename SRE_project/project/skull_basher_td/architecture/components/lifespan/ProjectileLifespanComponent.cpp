
#include "ProjectileLifespanComponent.hpp"
#include "../../scenes/GameObject.hpp"


// using namespace glm;
ProjectileLifespanComponent::ProjectileLifespanComponent(GameObject* gameObject) : Component(gameObject)
{
    start_life = std::chrono::steady_clock::now(); // initialises the internal timer
}

ProjectileLifespanComponent::~ProjectileLifespanComponent() = default;


void ProjectileLifespanComponent::update(float deltaTime){

    // update the internal timer
    std::chrono::steady_clock::time_point time_now = std::chrono::steady_clock::now();
    int time_elapsed_milli = std::chrono::duration_cast<std::chrono::milliseconds>(time_now - start_life).count();

    // check if lifespan is over and queue for deletion
    if (time_elapsed_milli > lifespan_millisec)
        gameObject->setQueuedForDeletion(true);

}

// set a different lifespan from the default
void ProjectileLifespanComponent::setLifespan(int lifeSpan){
    this->lifespan_millisec = lifeSpan;
}

// gets the lifespan of the object (not life remaining!)
int ProjectileLifespanComponent::getLifespan(){
    return lifespan_millisec;
}