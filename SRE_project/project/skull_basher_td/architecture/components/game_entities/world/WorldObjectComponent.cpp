
#include "WorldObjectComponent.hpp"

WorldObjectComponent::WorldObjectComponent(GameObject* gameObject) : Component(gameObject)
{

}

void WorldObjectComponent::setBuildable(bool isBuildable){
    // Set buildable status
    this->isbuildable = isBuildable;
}

void WorldObjectComponent::setIsPath(bool isPath){
    // set the block as a path block
    this->isPath = isPath;
}

bool WorldObjectComponent::getBuildableStatus(){
    return isbuildable;
}
bool WorldObjectComponent::getPathStatus(){
    return isPath;
}


WorldObjectComponent::~WorldObjectComponent()
{
}