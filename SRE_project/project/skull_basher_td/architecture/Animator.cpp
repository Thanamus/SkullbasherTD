//
// Created by kb on 09/12/2020.
//

#include "Animator.hpp"
Animator::Animator(GameObject* gameObject)
: Component(gameObject),
transform(gameObject->getComponent<Transform>().get()),
currentAnimation("none", new Animation()),
nextPosition(transform->position),
nextScale(transform->scale),
nextRotation(transform->rotation) {
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
    if(currentAnimation.first != "none" && currentAnimation.second->hasEnded(deltaTime)) {
        setAnimationState("none");
        return;
    }

    if(currentAnimation.first != "none") {
        bool playing = currentAnimation.second->updateFrame(deltaTime);
         if(playing) {
             auto keyframe = currentAnimation.second->getCurrentKeyframe();
             std::cout << currentAnimation.second->getCurrentKeyframeTime() << std::endl;
             // prep for next frame
             if(currentAnimation.second->getCurrentKeyframeTime() == 0) {
                 updateNextTransform(keyframe->translate, keyframe->scale, keyframe->rotate);
             }
             auto t = glm::smoothstep(0.0f, keyframe->timeDuration, currentAnimation.second->getCurrentKeyframeTime());
             transform->position = glm::mix(transform->position, nextPosition, t);
             transform->scale = glm::mix(transform->scale, nextScale, t);
             transform->rotation = glm::mix(transform->rotation, nextRotation, t);
         }
    }
}

const std::string &Animator::getAnimationState() const {
    return currentAnimation.first;
}

void Animator::updateNextTransform(glm::vec3 translate, glm::vec3 scale, glm::vec3 rotate) {
    nextPosition += translate;
    nextScale *= scale;
    nextRotation += rotate;
}
