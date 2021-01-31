
#include "CrystalHealth.hpp"
#include "../GameObject.hpp"

CrystalHealth::CrystalHealth(GameObject* gameObject) : HealthComponent(gameObject)
{

}

CrystalHealth::~CrystalHealth()
{
}

void CrystalHealth::HandleHealthChange() {
    if(this->getHealth() <= 0)
    {
        getGameObject()->getScene()->gameManager->levelRunning = false;
        getGameObject()->getScene()->gameManager->won = false;
    }
    else
    {
        //play sound or animate, or something
    }
}