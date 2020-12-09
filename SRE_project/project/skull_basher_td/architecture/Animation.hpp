//
// Created by kb on 08/12/2020.
//
#pragma once

#include <vector>
#include "glm/glm.hpp"

struct Keyframe {
    glm::vec3 position;
    glm::vec3 scale;
    glm::vec3 rotation;
    float timeDuration; // duration of transition to keyframe in seconds
};

class Animation {
private:
    std::vector<Keyframe> keyframes;
    unsigned int currentFrameIndex;
    float currentFrameTime;
    bool looping;

    void nextFrame();

public:
    Animation();

    unsigned int getCurrentFrameIndex() const;

    float getCurrentFrameTime() const;

    void addFrame(glm::vec3 position, glm::vec3 scale, glm::vec3 rotation, float timeDuration);

    const Keyframe* getCurrentFrame() const;

    bool updateFrame(float deltaTime);

    bool hasEnded();

    void reset();
};
