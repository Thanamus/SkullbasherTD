
#include "CrystalHealth.hpp"

CrystalHealth::CrystalHealth(GameObject* gameObject) : HealthComponent(gameObject)
{

}

CrystalHealth::~CrystalHealth()
{
}

void CrystalHealth::HandleHealthChange() {
    if(this->getHealth() <= 0)
    {
        //looose
    }
    else
    {
        //play sound or animate, or something
    }
}