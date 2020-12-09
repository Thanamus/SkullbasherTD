//
// Created by kb on 08/12/2020.
//

#include "Animation.hpp"

Animation::Animation(bool looping)
: keyframes(0), currentKeyframeIndex(0),
  currentKeyframeTime(0.f), looping(looping) {

}

Animation::Animation()
: keyframes(0), currentKeyframeIndex(0),
  currentKeyframeTime(0.f), looping(false) {

}


void Animation::addFrame(glm::vec3 translate, glm::vec3 scale, glm::vec3 rotate, float timeDuration)
{
    Keyframe data{};
    data.translate = translate;
    data.scale = scale;
    data.rotate = rotate;
    data.timeDuration = timeDuration;
    keyframes.push_back(data);
}

const Keyframe* Animation::getCurrentKeyframe() const
{
    if(!keyframes.empty()){
        return &keyframes[currentKeyframeIndex];
    }

    return nullptr;
}

bool Animation::updateFrame(float deltaTime) {
    // cant animate with no keyframes
    if(keyframes.empty())
        return false;

    // stops animating if not looping and reaches the end
    if(!looping && (currentKeyframeIndex == keyframes.size() - 1) && currentKeyframeTime + deltaTime > keyframes[currentKeyframeIndex].timeDuration)
        return false;

    currentKeyframeTime += deltaTime;

    if(currentKeyframeTime >= keyframes[currentKeyframeIndex].timeDuration) {
        currentKeyframeTime = 0.f;
        nextFrame();
        return true;
    }
}

void Animation::reset() {
    currentKeyframeIndex = 0;
    currentKeyframeTime = 0.f;
}

void Animation::nextFrame() {
    currentKeyframeIndex = (currentKeyframeIndex + 1) % keyframes.size(); // allows roll over to frame 0
}

unsigned int Animation::getCurrentKeyframeIndex() const {
    return currentKeyframeIndex;
}

float Animation::getCurrentKeyframeTime() const {
    return currentKeyframeTime;
}
