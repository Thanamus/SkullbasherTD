

#include "./PlaylistComponent.hpp"

#include "../GameObject.hpp"

class GameObject;

PlaylistComponent::PlaylistComponent(GameObject *gameObject) : Component(gameObject) {

}

void PlaylistComponent::addSoundEffect(std::string soundEffectCode, std::string soundEffectName){

    playlist[soundEffectCode] = soundEffectName; // add a sound effect name for a given sound effect code
}


bool PlaylistComponent::getSoundEffectName(std::string soundEffectCode, std::string &nameToReturn){

    // check if sound code + name pair exist in the map
    std::map<std::string, std::string>::iterator itr;
    itr = playlist.find(soundEffectCode);

    if (itr != playlist.end())
    {
        // code found, pass refernce back to caller
        nameToReturn = playlist[soundEffectCode];
        return true;
    } else {
        // sound effect code not found, output error
        std::cout << "ERROR: soundEffect: " << soundEffectCode << " for gameObject: " << gameObject->getName() <<
        " requested, but one was not found" << std::endl;        
        // *nameToReturn = nullptr;
        return false;
    }
    
}
