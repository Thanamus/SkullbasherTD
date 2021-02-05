
#include "ArrowLifespanComponent.hpp"
#include "../GameObject.hpp"


using namespace glm;
ArrowLifespanComponent::ArrowLifespanComponent(GameObject* gameObject) : Component(gameObject)
{
    start_life = std::chrono::steady_clock::now();
}

ArrowLifespanComponent::~ArrowLifespanComponent()
{

}


void ArrowLifespanComponent::update(float deltaTime){

    std::chrono::steady_clock::time_point time_now = std::chrono::steady_clock::now();
    int time_elapsed_milli = std::chrono::duration_cast<std::chrono::milliseconds>(time_now - start_life).count();

    if (time_elapsed_milli > lifespan_millisec)
        gameObject->deleteMe = true; 

}

// set a different lifespan from the default
void ArrowLifespanComponent::setLifespan(int lifeSpan){
    this->lifespan_millisec = lifeSpan;
}

// gets the lifespan of the object (not life remaining!)
int ArrowLifespanComponent::getLifespan(){
    return lifespan_millisec;
}