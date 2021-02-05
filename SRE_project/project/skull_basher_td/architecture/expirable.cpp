
#include "Expirable.hpp"
#include <cmath>


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

// set a different lifespan from the default
void Expirable::setLifespan(int lifeSpan){
    this->lifespan_millisec = lifeSpan;
}

// gets the lifespan of the object (not life remaining!)
int Expirable::getLifespan(){
    return lifespan_millisec;
}