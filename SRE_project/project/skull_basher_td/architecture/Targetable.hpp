#pragma once
class GameObject;

class Targetable {
public:
    ~Targetable() = default;

    GameObject *getTarget() const {
        return target;
    }

    void setTarget(GameObject *target_) {
        Targetable::target = target_;
    }

protected:
    GameObject* target = nullptr;
};