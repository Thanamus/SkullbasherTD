
#include "CrystalHealth.hpp"
#include "../GameObject.hpp"

CrystalHealth::CrystalHealth(GameObject* gameObject) : HealthComponent(gameObject)
{

}

CrystalHealth::~CrystalHealth()
{
}

void CrystalHealth::HandleHealthChange() {
    if (!GameManager::getInstance().levelRunning)
        return;

    if(this->getHealth() <= 0)
    {
        GameManager::getInstance().levelRunning = false;
        GameManager::getInstance().won = false;
        GameManager::getInstance().ToggleLockMouse();
    }
    else
    {
        //play sound or animate, or something
    }
}