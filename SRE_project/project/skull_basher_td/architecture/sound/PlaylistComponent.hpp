#pragma once


#include "../Component.hpp"
#include <string>
#include <map>

// Forward declarations
class Component;
class GameObject;

/*
    Playlist component enables 'sound effect codes' to be converted to 'sound effect names' for playback
*/


class PlaylistComponent : public Component {
public:

    explicit PlaylistComponent(GameObject *gameObject);

    void addSoundEffect(std::string soundEffectCode, std::string soundEffectName);

    bool getSoundEffectName(std::string soundEffectCode, std::string &nameToReturn);

private:
    std::map<std::string, std::string> playlist;
    
};