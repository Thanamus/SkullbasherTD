#pragma once

#include "HealthComponent.hpp"


#define GLM_ENABLE_EXPERIMENTAL


class CrystalHealthComponent : public HealthComponent  {
public:
    explicit CrystalHealthComponent(GameObject* gameObject);

    virtual ~CrystalHealthComponent();

    void HandleHealthChange() override;
};

