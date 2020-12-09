//
// Created by kb on 08/12/2020.
//

#include "Animation.hpp"

Animation::Animation() : keyframes(0), currentFrameIndex(0),
                         currentFrameTime(0.f)
{

}

void Animation::addFrame(glm::mat4 transform, float timeDuration)
{
    Keyframe data{};
    data.transform = transform;
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
