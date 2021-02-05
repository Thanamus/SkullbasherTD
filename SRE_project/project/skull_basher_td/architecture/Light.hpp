//
// Created by Morten Nobel Jørgensen on 2018-11-08.
//

#pragma once

#include <sre/Mesh.hpp>
#include <sre/Camera.hpp>
#include <sre/Light.hpp>
#include "GameObject.hpp"
#include <sre/Material.hpp>
#include "Transform.hpp"
#include "Updatable.hpp"

class Light : public Component {
public:
    Light(GameObject* gameObject);
    ~Light() { transform = nullptr; }
    void debugGUI() override ;

    sre::Light& getLight();
private:
    Transform* transform;

    sre::Light light;
};