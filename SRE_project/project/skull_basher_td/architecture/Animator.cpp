//
// Created by kb on 09/12/2020.
//

#include "Animator.hpp"
Animator::Animator(GameObject* gameObject)
: Component(gameObject), modelRenderer(gameObject->getComponent<ModelRenderer>().get()), currentAnimation("none", new Animation()) {
}

void Animator::addAnimation(const std::string& state, const std::shared_ptr<Animation>& animation) {
    animations.insert(std::make_pair(state, animation));
}

void Animator::setAnimationState(std::string state) {
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
    if(currentAnimation.first != "none") {
        bool newFrame = currentAnimation.second->updateFrame(deltaTime);
         if(newFrame) {
             auto keyframe = currentAnimation.second->getCurrentFrame();
             auto transform = modelRenderer->getTransform();
             auto t = glm::smoothstep(0.0f, 1.0f, currentAnimation.second->getCurrentFrameTime());
             transform->position = glm::mix(transform->position, keyframe->position, t);
             transform->scale = glm::mix(transform->scale, keyframe->scale, t);
             transform->rotation = glm::mix(transform->rotation, keyframe->rotation, t);
         }
    }
}
