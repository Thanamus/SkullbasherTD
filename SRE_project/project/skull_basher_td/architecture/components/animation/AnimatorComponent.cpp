//
// Created by kb on 09/12/2020.
//

#include "AnimatorComponent.hpp"
#include "../game_entities/world/WorldObjectComponent.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/euler_angles.hpp"
#include "glm/gtx/string_cast.hpp"

AnimatorComponent::AnimatorComponent(GameObject* gameObject)
: Component(gameObject), currentAnimation("none", new Animation())
{
    animations.insert(currentAnimation);
}

// adds animation to map
void AnimatorComponent::addAnimation(std::string state, std::shared_ptr<Animation> animation) {
    animations.insert(std::make_pair(std::move(state), std::move(animation)));
}

// sets the animation to be played
// returns immediately if it is playing
// finds and sets the current animation, and resets it to the start
void AnimatorComponent::setAnimationState(const std::string& state) {
    if (currentAnimation.first == state)
        return;

    auto animation = animations.find(state);
    if (animation != animations.end()) {
        currentAnimation.first = animation->first;
        currentAnimation.second = animation->second;
        currentAnimation.second->reset();
    }
}

// advances the animation frames, interpolating from the current SQT transform to the next
void AnimatorComponent::update(float deltaTime) {
    // if no animation on
    if(currentAnimation.first == "none")
        return;

    // if animation has ended
    if(currentAnimation.second->hasEnded(deltaTime)) {
        // end if the animation should only play once
        if(!currentAnimation.second->isLooping()) {
            setAnimationState("none");
            return;
        } else {
            // reset to start if animation should start playing again
            resetVectors();
        }
    }

    // if there is a next keyframe
    if(currentAnimation.second->updateFrame(deltaTime)) {
        // get keyframe
        auto keyframe = currentAnimation.second->getCurrentKeyframe();
        if(currentAnimation.second->getCurrentKeyframeTime() == 0 + deltaTime) {
            // at start of frame, compute new target vectors
            targetTransform = initTransformData(currentTransform.position + keyframe->translate,
                                                currentTransform.scale * keyframe->scale,
                                                currentTransform.rotation + keyframe->rotate);
        }

        // normalize the elapsed time based on the duration of the keyframe
        auto t = glm::smoothstep(0.0f, keyframe->timeDuration, currentAnimation.second->getCurrentKeyframeTime());
        auto oldRot = currentTransform.rotation;
        // interpolate the SQT data
        currentTransform = initTransformData(
                glm::mix(startTransform.position, targetTransform.position, t),
                glm::mix(startTransform.scale, targetTransform.scale, t),
                glm::mix(startTransform.rotation, targetTransform.rotation, t));
        // update the matrix with the new SQT data (slightly less efficient, but easier to implement)
        updateSQTMatrix();
    }
}

const std::string &AnimatorComponent::getAnimationState() const {
    return currentAnimation.first;
}

// conveniently sets the transform data, basically replaces the struct constructor
TransformData AnimatorComponent::initTransformData(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation) {
    return {position, scale, rotation};
}

// converts XYZ rotation to quaterion
glm::mat4 AnimatorComponent::getQuaternionRotation(glm::vec3 rotation) {
    glm::mat4 rotZ = glm::eulerAngleZ(glm::radians(rotation.z));
    glm::mat4 rotY = glm::eulerAngleY(glm::radians(rotation.y));
    glm::mat4 rotX = glm::eulerAngleX(glm::radians(rotation.x));
    return rotZ*rotY*rotX;
}

// calculates matrix based on current transform data
void AnimatorComponent::updateSQTMatrix() {
    glm::mat4 translateMat = glm::translate(glm::mat4(1), currentTransform.position);
    glm::mat4 scaleMat = glm::scale(glm::mat4(1), currentTransform.scale);
    SQTMatrix = translateMat*getQuaternionRotation(currentTransform.rotation)*scaleMat;
}

// resets everything to have a "clean start"
void AnimatorComponent::resetVectors() {
    startTransform = initTransformData(glm::vec3(0), glm::vec3(1), glm::vec3(0));
    currentTransform = startTransform;
    targetTransform = initTransformData(glm::vec3(0), glm::vec3(1), glm::vec3(0));
}

glm::mat4 AnimatorComponent::getSQTMatrix() const {
    return SQTMatrix;
}

std::shared_ptr<Animation> AnimatorComponent::getAnimationForState(const std::string& state) {
    return animations.find(state)->second;
};


