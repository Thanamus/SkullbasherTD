
#include "WorldObject.hpp"

WorldObject::WorldObject(GameObject* gameObject) : Component(gameObject)
{

}

void WorldObject::setBuildable(bool isBuildable){
    this->isbuildable = isBuildable;
}

void WorldObject::setPath(bool isPath){
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