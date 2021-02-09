//
// Created by kb on 31/01/2021.
//

#include "CoinComponent.hpp"

CoinComponent::CoinComponent(GameObject* gameObject) : Component(gameObject) {
}

CoinComponent::~CoinComponent() {
}

float CoinComponent::getMoney() const {
    return money;
}

void CoinComponent::setMoney(float money) {
    CoinComponent::money = money;
}
