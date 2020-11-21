//
// Created by Morten Nobel-Jørgensen on 12/09/2017.
//
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>
#include "Asteroid.hpp"
#include "sre/Renderer.hpp"
#include "ctime"
#include "Laser.hpp"
#include "AsteroidFactory.hpp"

Asteroid::Asteroid(const sre::Sprite &_sprite, Size _size, glm::vec2 _position) : GameObject(_sprite)
{
    scale = glm::vec2(0.5f, 0.5f);
    winSize = sre::Renderer::instance->getDrawableSize();
    size = _size;
    switch (size)
    {
    case BIG:
        radius = 24;
        break;
    case MEDIUM:
        radius = 12;
        break;
    case SMALL:
        radius = 6;
        break;
    default:
        break;
    }
    // rand rotation
    (((rand() % 2) + 1) == 1) ? rotateCW = true : rotateCCW = true;

    // rand direction (up/down)
    float xdir = (((rand() % 2) + 1) == 1) ? 1.f : -1.f;
    float ydir = (((rand() % 2) + 1) == 1) ? 1.f : -1.f;

    if (_position == glm::vec2(NULL, NULL))
    {
        // idk hopefully random enough?
        float ver = (((rand() % 2) + 1) == 1) ? (0.01f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 0.2f))) : (0.8f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 0.99f)));
        float hor = (((rand() % 2) + 1) == 1) ? (0.01f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 0.2f))) : (0.8f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / 0.99f)));
        position = winSize * glm::vec2(ver, hor);
    }
    else
    {
        position = _position;
    }

    // rand speed
    float x = minSpeed + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / maxSpeed));
    float y = minSpeed + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / maxSpeed));
    velocity = glm::vec2(x * xdir, y * ydir);
}

std::shared_ptr<GameObject> Asteroid::detectCollision()
{
    for (auto go : AsteroidsGame::getInstance()->getGameObjects())
    {
        if (go.get() == this)
            continue;
        //get the object positions
        float posx = go->getPosition().x;
        float posy = go->getPosition().y;
        // let's check if it's a collidable!
        if (std::shared_ptr<Collidable> coll = std::dynamic_pointer_cast<Collidable>(go))
        {
            float x_dist = posx - this->position.x;
            float y_dist = posy - this->position.y;
            if (sqrt(x_dist * x_dist + y_dist * y_dist) < (this->radius + coll->getRadius()))
            {
                return go;
            }
        }
    }
    return nullptr;
}

void Asteroid::update(float deltaTime)
{
    std::shared_ptr<GameObject> collider = detectCollision();
    if (collider != nullptr)
    {
        onCollision(collider);
    }

    rotation += rotationSpeed * ((rotateCW) ? 1 : -1) * deltaTime;
    position += velocity * deltaTime;
    if (position.x < 0)
    {
        position.x += winSize.x;
    }
    else if (position.x > winSize.x)
    {
        position.x -= winSize.x;
    }
    if (position.y < 0)
    {
        position.y += winSize.y;
    }
    else if (position.y > winSize.y)
    {
        position.y -= winSize.y;
    }
}

void Asteroid::onCollision(std::shared_ptr<GameObject> other)
{
    if (std::shared_ptr<Laser> las = std::dynamic_pointer_cast<Laser>(other))
    {
        std::cout << "EH OH LASERONNEEEEEE" << std::endl;
        AsteroidsGame::getInstance()->incScore();
        switch (this->size)
        {
        case BIG:
            std::cout << "EH EL BIGGO MMOMOMMOM" << std::endl;
            AsteroidFactory::getInstance()->makeAsteroid(MEDIUM, BROWN, position);
            AsteroidFactory::getInstance()->makeAsteroid(MEDIUM, GREY, position);
            break;
        case MEDIUM:
            AsteroidFactory::getInstance()->makeAsteroid(SMALL, BROWN, position);
            AsteroidFactory::getInstance()->makeAsteroid(SMALL, GREY, position);
            break;
        default:
            break;
        }
        AsteroidsGame::getInstance()->queueToDel(shared_from_this());
    }
}
