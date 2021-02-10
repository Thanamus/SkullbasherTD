#pragma once
class GameObject;

class Targetable {
public:
    ~Targetable() { target = nullptr; };

    GameObject *getTarget() const {
        return target;
    }

    void setTarget(GameObject *target_) {
        Targetable::target = target_;
    }

protected:
    GameObject* target = nullptr;
};