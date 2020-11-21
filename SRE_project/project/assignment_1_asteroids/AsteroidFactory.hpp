#pragma once
#include "Asteroid.hpp"
#include <vector>

class AsteroidFactory
{
public:
    static AsteroidFactory *getInstance();
    void makeAsteroid(Asteroid::Size size, Asteroid::Color color, glm::vec2 _position = glm::vec2(NULL, NULL));

private:
    static AsteroidFactory *instance;
    AsteroidFactory();
};