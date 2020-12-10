//
// Created by kb on 09/12/2020.
//

#include "Animator.hpp"
Animator::Animator(GameObject* gameObject)
: Component(gameObject),
transform(gameObject->getComponent<Transform>().get()),
currentAnimation("none", new Animation())
{
    startTransform.position = transform->position;
    startTransform.scale = transform->scale;
    startTransform.rotation = transform->rotation;
    nextTransform = startTransform;
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
    std::cout << "updating..." << std::endl;
    if(currentAnimation.first != "none" && currentAnimation.second->hasEnded(deltaTime)) {
        transform->position = startTransform.position;
        transform->scale = startTransform.scale;
        transform->rotation = startTransform.rotation;
        // end if the animation should only play once
        if(!currentAnimation.second->isLooping()) {
            setAnimationState("none");
            return;
        }
        currentAnimation.second->updateFrame(deltaTime); // go back to initial frame

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
             transform->position = glm::mix(transform->position, nextTransform.position, t);
             transform->scale = glm::mix(transform->scale, nextTransform.scale, t);
             transform->rotation = glm::mix(transform->rotation, nextTransform.rotation, t);
         }
    }
}

const std::string &Animator::getAnimationState() const {
    return currentAnimation.first;
}

void Animator::updateNextTransform(glm::vec3 translate, glm::vec3 scale, glm::vec3 rotate) {
    nextTransform.position += translate;
    nextTransform.scale *= scale;
    nextTransform.rotation += rotate;
}
