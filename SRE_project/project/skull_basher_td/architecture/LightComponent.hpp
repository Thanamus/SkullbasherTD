//
// Created by Morten Nobel Jørgensen on 2018-11-08.
//

#pragma once

#include <sre/Mesh.hpp>
#include <sre/Camera.hpp>
#include <sre/Light.hpp>
#include "GameObject.hpp"
#include <sre/Material.hpp>
#include "TransformComponent.hpp"
#include "Updatable.hpp"

class LightComponent : public Component {
public:
    LightComponent(GameObject* gameObject);
    ~LightComponent() { transform = nullptr; }
    void debugGUI() override ;

    sre::Light& getLight();
private:
    TransformComponent* transform;

    sre::Light light;
};