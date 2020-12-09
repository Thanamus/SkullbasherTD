//
// Created by kb on 08/12/2020.
//
#pragma once

#include <vector>
#include "glm/glm.hpp"

struct Keyframe {
    glm::vec3 translate;
    glm::vec3 scale;
    glm::vec3 rotate;
    float timeDuration; // duration of transition to keyframe in seconds
};

class Animation {
private:
    std::vector<Keyframe> keyframes;
    unsigned int currentKeyframeIndex;
    float currentKeyframeTime;
    bool looping;

    void nextFrame();

public:
    Animation();
    explicit Animation(bool looping);

    unsigned int getCurrentKeyframeIndex() const;

    float getCurrentKeyframeTime() const;

    void addFrame(glm::vec3 translate, glm::vec3 scale, glm::vec3 rotate, float timeDuration);

    const Keyframe* getCurrentKeyframe() const;

    bool updateFrame(float deltaTime);

    bool hasEnded();

    void reset();
};
