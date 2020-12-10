//
// Created by kb on 09/12/2020.
//
#pragma once
#include "Animation.hpp"
#include "Transform.hpp"
#include "ModelRenderer.hpp"

struct TransformVectors {
    glm::vec3 position;
    glm::vec3 scale;
    glm::vec3 rotation;
};

class Animator : public Component, public Updatable {
public:
    explicit Animator(GameObject* gameObject);
    ~Animator() override = default;

    void update(float deltaTime) override;

    void addAnimation(std::string state, std::shared_ptr<Animation> animation);

    void setAnimationState(const std::string& state);
    const std::string& getAnimationState() const;

    void updateNextTransform(glm::vec3 translate, glm::vec3 scale, glm::vec3 rotate);
private:
    Transform* transform;
    std::pair<std::string, std::shared_ptr<Animation>> currentAnimation;
    std::map<std::string, std::shared_ptr<Animation>> animations; // animations are given a name; can be later defined in an an animation json/script/whatever
    TransformVectors nextTransform{};
    TransformVectors startTransform{};
};
