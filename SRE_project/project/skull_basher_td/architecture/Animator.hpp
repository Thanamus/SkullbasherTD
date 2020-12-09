//
// Created by kb on 09/12/2020.
//
#pragma once
#include "Animation.hpp"
#include "Transform.hpp"
#include "ModelRenderer.hpp"

class Animator : public Component {
public:
    explicit Animator(GameObject* gameObject);
    ~Animator() override = default;

    void update(float deltaTime);

    void addAnimation(const std::string& state, const std::shared_ptr<Animation>& animation);

    void setAnimationState(std::string state);
private:
    ModelRenderer* modelRenderer;
    std::pair<std::string, std::shared_ptr<Animation>> currentAnimation;
    std::map<std::string, std::shared_ptr<Animation>> animations; // animations are given a name; can be later defined in an an animation json/script/whatever
};
