#pragma once

#include "HealthComponent.hpp"


#define GLM_ENABLE_EXPERIMENTAL


class CrystalHealth : public HealthComponent  {
public:
    explicit CrystalHealth(GameObject* gameObject);

    virtual ~CrystalHealth();

    void HandleHealthChange() override;
};

