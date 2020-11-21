#pragma once

#include <SDL_events.h>
#include <string>
#include "sre/Sprite.hpp"
#include "sre/SpriteBatch.hpp"
#include "AsteroidsGame.hpp"

class GameObject : public std::enable_shared_from_this<GameObject>
{
public:
    explicit GameObject(sre::Sprite sprite);
    virtual ~GameObject();
    virtual void render(sre::SpriteBatch::SpriteBatchBuilder &spriteBatchBuilder);
    virtual void update(float deltaTime);
    virtual void onKey(SDL_Event &keyEvent);
    glm::vec2 getPosition();
    float getRotation();

protected:
    glm::vec2 position;
    glm::vec2 scale;
    sre::Sprite sprite;
    float rotation;

    friend class AsteroidsGame;
};
