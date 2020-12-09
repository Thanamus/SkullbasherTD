//
// Created by kb on 08/12/2020.
//
#pragma once

#include <vector>
#include "glm/glm.hpp"

struct Keyframe {
    glm::mat4 transform;
    float timeDuration; // duration of transition to keyframe in seconds
};

class Animation {
private:
    void nextFrame();

    // Stores all frames for our animation.
    std::vector<Keyframe> keyframes;
    unsigned int currentFrameIndex;
    float currentFrameTime;
public:
    Animation();

    unsigned int getCurrentFrameIndex() const;

    float getCurrentFrameTime() const;

    void addFrame(glm::mat4 transform, float timeDuration);

    const Keyframe* getCurrentFrame() const;

    bool updateFrame(float deltaTime);

    void reset();
};
