//
// Created by kb on 09/12/2020.
//

#include "Animator.hpp"
Animator::Animator(GameObject* gameObject)
: Component(gameObject), modelRenderer(gameObject->getComponent<ModelRenderer>().get()), currentAnimation("none", new Animation()) {
}

void Animator::addAnimation(const std::string& state, std::shared_ptr<Animation> animation) {
    auto inserted = animations.insert(std::make_pair(state, animation));

    if(currentAnimation.first == "none") {
        setAnimationState(state);
    }
}
