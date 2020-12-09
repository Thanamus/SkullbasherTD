//
// Created by kb on 08/12/2020.
//

#include "Animation.hpp"

Animation::Animation() : keyframes(0), currentFrameIndex(0),
                         currentFrameTime(0.f), looping(false)
{

}

void Animation::addFrame(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation, float timeDuration)
{
    Keyframe data{};
    data.position = position;
    data.scale = scale;
    data.rotation = position;
    data.timeDuration = timeDuration;
    keyframes.push_back(data);
}

const Keyframe* Animation::getCurrentFrame() const
{
    if(!keyframes.empty()){
        return &keyframes[currentFrameIndex];
    }

    return nullptr;
}

bool Animation::updateFrame(float deltaTime) {
    if(!keyframes.empty()) {
        currentFrameTime += deltaTime;
        // stops animating if looping is false
        if(hasEnded())
            return false;
        if(currentFrameTime >= keyframes[currentFrameIndex].timeDuration) {
            currentFrameTime = 0.f;
            nextFrame();
            return true;
        }
    }
    return false;
}

void Animation::reset() {
    currentFrameIndex = 0;
    currentFrameTime = 0.f;
}

void Animation::nextFrame() {
    currentFrameIndex = (currentFrameIndex + 1) % keyframes.size(); // allows roll over to frame 0
}

unsigned int Animation::getCurrentFrameIndex() const {
    return currentFrameIndex;
}

float Animation::getCurrentFrameTime() const {
    return currentFrameTime;
}

bool Animation::hasEnded() {
    return !looping && (currentFrameIndex == (keyframes.size() - 1));
}
