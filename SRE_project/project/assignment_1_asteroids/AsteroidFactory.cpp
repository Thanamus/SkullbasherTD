#include "AsteroidFactory.hpp"
#include "cstring"
#include "sre/SpriteAtlas.hpp"

AsteroidFactory *AsteroidFactory::instance = nullptr;

AsteroidFactory *AsteroidFactory::getInstance()
{
    if (!instance)
        instance = new AsteroidFactory();
    return instance;
}

AsteroidFactory::AsteroidFactory()
{
}

void AsteroidFactory::makeAsteroid(Asteroid::Size size, Asteroid::Color color, glm::vec2 _position)
{
    // given that sprite follow a naming convention, I can simply build the correct string for the sprite
    std::string sprite_color = (color == Asteroid::BROWN) ? "Brown" : "Grey"; // could easily be converted to switch statement
    std::string sprite_size;
    switch (size)
    {
    case Asteroid::SMALL:
        sprite_size = "small";
        break;
    case Asteroid::MEDIUM:
        sprite_size = "med";
        break;
    default:
        sprite_size = "big";
    }
    std::string sprite_variant = std::to_string(rand() % 2 + 1); // just 2 variants for each SAREBBE BELLO SE FOSSERO CONSECUTIVE
    std::string sprite_name = "meteor" + sprite_color + "_" + sprite_size + "1.png";
    sre::Sprite sprite = AsteroidsGame::getInstance()->getAtlas()->get(sprite_name);
    AsteroidsGame::getInstance()->queueToAdd(std::make_shared<Asteroid>(sprite, size, _position));
};
