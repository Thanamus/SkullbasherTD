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

// adds a new frame to the animation
void Animation::addFrame(glm::vec3 translate, glm::vec3 scale, glm::vec3 rotate, float timeDuration) {
    Keyframe data{};
    data.translate = translate;
    data.scale = scale;
    data.rotate = rotate;
    data.timeDuration = timeDuration;
    keyframes.push_back(data);
}

// returns current keyframe
const Keyframe* Animation::getCurrentKeyframe() const
{
    if(!keyframes.empty()){
        return &keyframes[currentKeyframeIndex];
    }

    return nullptr;
}

// updates the current keyframe based on time passed, and either quits (if ended & not looping) or keeps advancing frames
// returns true if there is a next frame, false if not
bool Animation::updateFrame(float deltaTime) {
    // cant animate with no keyframes
    if(keyframes.empty())
        return false;

    // stops animating if not looping and reaches the end
    if(!looping && hasEnded(deltaTime))
        return false;

    // adds time passed
    currentKeyframeTime += deltaTime;

    // checks if it is time to pass to the next keyframe
    if(currentKeyframeTime >= keyframes[currentKeyframeIndex].timeDuration) {
        currentKeyframeTime = 0.f;
        nextFrame();
        return true;
    }
}

// resets to start
void Animation::reset() {
    currentKeyframeIndex = 0;
    currentKeyframeTime = 0.f;
}

// gets the next frame with rollover (for looping animations)
void Animation::nextFrame() {
    currentKeyframeIndex = (currentKeyframeIndex + 1) % keyframes.size(); // allows roll over to frame 0
}

unsigned int Animation::getCurrentKeyframeIndex() const {
    return currentKeyframeIndex;
}

float Animation::getCurrentKeyframeTime() const {
    return currentKeyframeTime;
}

// returns true it's the last frame AND if the next step would exceed the frame duration (frame has finished playing)
bool Animation::hasEnded(float deltaTime) {
    // the last keyframe has finished playing
    return (currentKeyframeIndex == keyframes.size() - 1) && (currentKeyframeTime + deltaTime > keyframes[currentKeyframeIndex].timeDuration);
}

bool Animation::isLooping() const {
    return looping;
}

void Animation::setLooping(bool looping) {
    Animation::looping = looping;
}
