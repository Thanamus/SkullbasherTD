//
// Created by Morten Nobel-Jørgensen on 12/09/2017.
//
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>
#include "SpaceShip.hpp"
#include "sre/Renderer.hpp"
#include "Asteroid.hpp"
#include "Laser.hpp"

SpaceShip::SpaceShip(const sre::Sprite &_sprite, const sre::Sprite &_deadSprite) : GameObject(_sprite)
{
    scale = glm::vec2(0.5f, 0.5f);
    deadSprite = _deadSprite;
    winSize = sre::Renderer::instance->getDrawableSize();
    radius = 23;
    position = winSize * 0.5f;
    velocity = glm::vec2(0.0f, 0.0f);
}

void SpaceShip::update(float deltaTime)
{
    if (!alive)
        return;
    std::shared_ptr<GameObject> collider = detectCollision();
    if (collider != nullptr)
    {
        onCollision(collider);
    }

    if (thrust)
    {
        float acceleration = deltaTime * thrustPower;
        glm::vec2 direction = glm::rotateZ(glm::vec3(0, acceleration, 0), glm::radians(rotation));
        velocity += direction;
        float speed = glm::length(velocity);
        if (speed > maxSpeed)
        {
            velocity = velocity * (maxSpeed / speed);
        }
    }
    else
    {
        velocity = velocity * (1.0f - drag * deltaTime);
    }
    position += velocity * deltaTime;
    if (rotateCCW)
    {
        rotation += deltaTime * rotationSpeed;
    }
    if (rotateCW)
    {
        rotation -= deltaTime * rotationSpeed;
    }

    // wrap around
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
void SpaceShip::onCollision(std::shared_ptr<GameObject> other)
{
    if (std::shared_ptr<Asteroid> ast = std::dynamic_pointer_cast<Asteroid>(other))
    {
        sprite = deadSprite;
        velocity = glm::vec2(0.0f, 0.0f);
        alive = false;
        AsteroidsGame::getInstance()->setGameOver();
    }
}

std::shared_ptr<GameObject> SpaceShip::detectCollision()
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

void SpaceShip::onKey(SDL_Event &keyEvent)
{
    if (!alive)
        return;
    switch (keyEvent.key.keysym.sym)
    {
    case SDLK_UP:
        thrust = keyEvent.type == SDL_KEYDOWN;
        break;
    case SDLK_LEFT:
        rotateCCW = keyEvent.type == SDL_KEYDOWN;
        break;
    case SDLK_RIGHT:
        rotateCW = keyEvent.type == SDL_KEYDOWN;
        break;
    case SDLK_SPACE:
        shootLaser();
        break;
    default:
        break;
    }
}

void SpaceShip::shootLaser()
{
    auto sprite = AsteroidsGame::getInstance()->getAtlas()->get("laserRed06.png");
    AsteroidsGame::getInstance()->queueToAdd(std::make_shared<Laser>(sprite, this));
}
