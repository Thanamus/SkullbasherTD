
#include "WorldObject.hpp"
#include "./GameObject.hpp"

WorldObject::WorldObject(GameObject* gameObject) : Component(gameObject)
{

}

void WorldObject::setBuildable(bool isBuildable){
    // Set buildable status
    this->isbuildable = isBuildable;
}

void WorldObject::setIsPath(bool isPath){
    // set the block as a path block
    this->isPath = isPath;
}

bool WorldObject::getBuildableStatus(){
    return isbuildable;
}
bool WorldObject::getPathStatus(){
    return isPath;
}


WorldObject::~WorldObject()
{
}