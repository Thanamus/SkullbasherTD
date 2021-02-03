//
// Created by kb on 09/12/2020.
//

#include "Animator.hpp"
#include "WorldObject.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/euler_angles.hpp"
#include "glm/gtx/string_cast.hpp"

Animator::Animator(GameObject* gameObject)
: Component(gameObject), currentAnimation("none", new Animation())
{
    animations.insert(currentAnimation);
}

void Animator::addAnimation(std::string state, std::shared_ptr<Animation> animation) {
    animations.insert(std::make_pair(std::move(state), std::move(animation)));
}

void Animator::setAnimationState(const std::string& state) {
    if (currentAnimation.first == state)
        return;

    auto animation = animations.find(state);
    if (animation != animations.end()) {
        currentAnimation.first = animation->first;
        currentAnimation.second = animation->second;
        currentAnimation.second->reset();
    }
}

void Animator::update(float deltaTime) {
    if(currentAnimation.first == "none")
        return;

    if(currentAnimation.second->hasEnded(deltaTime)) {
        // end if the animation should only play once
        if(!currentAnimation.second->isLooping()) {
            setAnimationState("none");
            return;
        } else {
            resetVectors();
        }
    }

    if(currentAnimation.second->updateFrame(deltaTime)) {
        // get keyframe
        auto keyframe = currentAnimation.second->getCurrentKeyframe();
        if(currentAnimation.second->getCurrentKeyframeTime() == 0 + deltaTime) {
            // at start of frame, compute new target vectors
            targetTransform = initTransformData(currentTransform.position + keyframe->translate,
                                                currentTransform.scale * keyframe->scale,
                                                currentTransform.rotation + keyframe->rotate);
        }

        auto t = glm::smoothstep(0.0f, keyframe->timeDuration, currentAnimation.second->getCurrentKeyframeTime());
//        auto t = currentAnimation.second->getCurrentKeyframeTime();
        auto oldRot = currentTransform.rotation;
        currentTransform = initTransformData(
                glm::mix(startTransform.position, targetTransform.position, t),
                glm::mix(startTransform.scale, targetTransform.scale, t),
                glm::mix(startTransform.rotation, targetTransform.rotation, t));
        std::cout << "cur_rot " << currentTransform.rotation.x << std::endl;
        std::cout << "t " << t << std::endl;
        std::cout << "time " << currentAnimation.second->getCurrentKeyframeTime() << std::endl;
        updateSQTMatrix();
    }
}

const std::string &Animator::getAnimationState() const {
    return currentAnimation.first;
}

TransformData Animator::initTransformData(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation) {
    return {position, scale, rotation};
}


glm::mat4 Animator::getQuaternionRotation(glm::vec3 rotation) {
    glm::mat4 rotZ = glm::eulerAngleZ(glm::radians(rotation.z));
    glm::mat4 rotY = glm::eulerAngleY(glm::radians(rotation.y));
    glm::mat4 rotX = glm::eulerAngleX(glm::radians(rotation.x));
    return rotZ*rotY*rotX;
}

void Animator::updateSQTMatrix() {
    glm::mat4 translateMat = glm::translate(glm::mat4(1), currentTransform.position);
    glm::mat4 scaleMat = glm::scale(glm::mat4(1), currentTransform.scale);
    SQTMatrix = translateMat*getQuaternionRotation(currentTransform.rotation)*scaleMat;
}

void Animator::resetVectors() {
    startTransform = initTransformData(glm::vec3(0), glm::vec3(1), glm::vec3(0));
    currentTransform = startTransform;
    targetTransform = initTransformData(glm::vec3(0), glm::vec3(1), glm::vec3(0));
}

glm::mat4 Animator::getSQTMatrix() const {
    return SQTMatrix;
}


