//
// Created by kb on 09/12/2020.
//
#pragma once
#include "Animation.hpp"
#include "GameObject.hpp"
#include "Updatable.hpp"
#include "Component.hpp"

struct TransformData {
    glm::vec3 position;
    glm::vec3 scale;
    glm::vec3 rotation;
};

class Animator : public Component, public Updatable {
public:
    explicit Animator(GameObject* gameObject);

    void update(float deltaTime) override;

    void addAnimation(std::string state, std::shared_ptr<Animation> animation);

    void setAnimationState(const std::string& state);
    const std::string& getAnimationState() const;

    glm::mat4 getSQTMatrix() const;

private:
    std::pair<std::string, std::shared_ptr<Animation>> currentAnimation;
    std::map<std::string, std::shared_ptr<Animation>> animations; // animations are given a name; can be later defined in an an animation json/script/whatever
    glm::mat4 SQTMatrix = glm::mat4(1.0f);

    TransformData startTransform{glm::vec3(0), glm::vec3(1), glm::vec3(0) };
    TransformData currentTransform{glm::vec3(0), glm::vec3(1), glm::vec3(0) };
    TransformData targetTransform{glm::vec3(0), glm::vec3(1), glm::vec3(0) };

    static glm::mat4 getQuaternionRotation(glm::vec3 rotation);
    void updateSQTMatrix();
    void resetVectors();

    static TransformData initTransformData(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation);
};
