
#include "CrystalHealth.hpp"
#include "../GameObject.hpp"
#include "../../GameManager.hpp"


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
        GameManager::getInstance().toggleWinState(false);
    }
    else
    {
        //play sound or animate, or something
    }
}