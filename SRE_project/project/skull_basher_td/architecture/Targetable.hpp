//
// Created by Morten Nobel Jørgensen on 2018-11-08.
//

#pragma once

class Targetable {
public:
    ~Targetable() = default;

    GameObject *getTarget() const {
        return target;
    }

    void setTarget(GameObject *target) {
        Targetable::target = target;
    }

private:
    GameObject* target = nullptr;
};