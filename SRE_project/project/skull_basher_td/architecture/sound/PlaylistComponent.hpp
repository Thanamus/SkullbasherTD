#pragma once


#include "../Component.hpp"
#include <string>
#include <map>

// Forward declarations
class Component;
class GameObject;

// struct Playlist{

//     std::string soundEffectCode;
//     std::string soundEffectName;
// };

class PlaylistComponent : public Component {
public:
    // explicit CustomCollisionHandler(GameObject *gameObject) : Component(gameObject));
    explicit PlaylistComponent(GameObject *gameObject);

    void addSoundEffect(std::string soundEffectCode, std::string soundEffectName);

    // TODO find out if this can be made a weak_ptr
    bool getSoundEffectName(std::string soundEffectCode, std::string* const &nameToReturn);
    // bool getSoundEffectName(std::string soudnEffectCode, std::string nameToReturn);
    // bool getSoundEffectName(std::string soudnEffectCode, std::weak_ptr<std::string> const &nameToReturn);

private:
    // Playist
    // playlist a;

    std::map<std::string, std::string> playlist;
    
};