//
// Created by Morten Nobel Jørgensen on 2018-11-06.
//

#pragma once

#include <memory>

// forward declaration
class GameObject;

class Component {
public:
    ~Component();
    GameObject* getGameObject();
    bool active = true;

    virtual void debugGUI();

protected:
    explicit Component(GameObject* gameObject);
    GameObject* gameObject;

    friend class GameObject;
};