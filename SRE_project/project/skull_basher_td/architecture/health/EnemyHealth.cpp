
#include "EnemyHealth.hpp"
#include "../GameObject.hpp"
#include "../../GameManager.hpp"
#include "../sound/SourceManager.hpp"
#include "../Transform.hpp"

EnemyHealth::EnemyHealth(GameObject* gameObject) : HealthComponent(gameObject)
{

}

EnemyHealth::~EnemyHealth()
{
}

void EnemyHealth::HandleHealthChange() {
    if (!GameManager::getInstance().levelRunning)
        return;

    if(this->getHealth() <= 0)
    {
        SourceManager::Get()->playMyJam("deathd.wav", this->gameObject->getComponent<Transform>()->position, 20);
        this->getGameObject()->deleteMe = true;
    }
    else
    {
        //play sound or animate, or something
    }
}