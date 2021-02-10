
#include "CrystalHealthComponent.hpp"
#include "../../scenes/GameObject.hpp"
#include "../../managers/GameManager.hpp"
#include "../../music/MusicBuffer.hpp"

CrystalHealthComponent::CrystalHealthComponent(GameObject* gameObject) : HealthComponent(gameObject)
{

}

CrystalHealthComponent::~CrystalHealthComponent()
{
}

void CrystalHealthComponent::HandleHealthChange() {
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