#pragma once

#include "HealthComponent.hpp"


#define GLM_ENABLE_EXPERIMENTAL


class EnemyHealth : public HealthComponent  {
public:
    explicit EnemyHealth(GameObject* gameObject);

    virtual ~EnemyHealth();

    void HandleHealthChange() override;
};

