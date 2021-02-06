

#include "./PlaylistComponent.hpp"

#include "../GameObject.hpp"

class GameObject;

PlaylistComponent::PlaylistComponent(GameObject *gameObject) : Component(gameObject) {

}

void PlaylistComponent::addSoundEffect(std::string soundEffectCode, std::string soundEffectName){
    // playlist.insert(soundEffectCode, soundEffectName); // This is causing an illegal indirectin error
    playlist[soundEffectCode] = soundEffectName;
}

// bool PlaylistComponent::getSoundEffectName(std::string soundEffectCode, std::weak_ptr<std::string> const &nameToReturn){
bool PlaylistComponent::getSoundEffectName(std::string soundEffectCode, std::string* const &nameToReturn){
// bool PlaylistComponent::getSoundEffectName(std::string soundEffectCode, std::string nameToReturn){

    std::map<std::string, std::string>::iterator itr;

    itr = playlist.find(soundEffectCode);

    if (itr != playlist.end())
    {
        // TODO check if this can be a weak ptr instead
        *nameToReturn = playlist[soundEffectCode];
        return true;
    } else {
        std::cout << "ERROR: soundEffect: " << soundEffectCode << " for gameObject: " << gameObject->getName() <<
        " requested, but one was not found" << std::endl;        
        // *nameToReturn = nullptr;
        return false;
    }
    
}
