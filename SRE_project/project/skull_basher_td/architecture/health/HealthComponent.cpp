
#include <imgui.h>
#include "HealthComponent.hpp"

HealthComponent::HealthComponent(GameObject* gameObject) : Component(gameObject)
{

}

HealthComponent::~HealthComponent()
{
}

float HealthComponent::getHealth()
{
    return health;
}

void HealthComponent::setHealth(float health)
{
    this->health = health;
    HandleHealthChange();
}

void HealthComponent::addHealth(float health)
{
    this->health += health;
    HandleHealthChange();
}

void HealthComponent::decreaseHealth(float health)
{
    this->health -= health;
    HandleHealthChange();
}

void HealthComponent::HandleHealthChange()
{

}

void HealthComponent::debugGUI() {
    ImGui::PushID(this);
    if (ImGui::TreeNode("HealthComponent")) {
        ImGui::DragFloat("Health", &health);
        ImGui::TreePop();
    }
    ImGui::PopID();
}
