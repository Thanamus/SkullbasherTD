//
// Created by Morten Nobel Jørgensen on 2018-11-08.
//

#include "Camera.hpp"
#include "sre/Renderer.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "WorldObject.hpp"
#include "ModelRenderer.hpp"
#include "PersonController.hpp"
#include "../GameManager.hpp"


using namespace std;
using namespace glm;
#include <functional>   // std::ref
Camera::Camera(GameObject * gameObject)
:Component(gameObject)
{
    transform = gameObject->getComponent<Transform>().get();
    colors = {
            {1, 0, 0, 0.5},
            {0, 1, 0, 0.5},
            {0, 0, 1, 0.5},
            {1, 1, 1, 0.5}};
    materials = {sre::Shader::getUnlit()->createMaterial()};
}

void Camera::bind() {
    if (perspective){
        camera.setPerspectiveProjection(fovY,nearPlane, farPlane);
    } else {
        camera.setOrthographicProjection(size,nearPlane,farPlane);
    }

    camera.setViewTransform(glm::inverse(transform->globalTransform()));
}

void Camera::debugGUI() {
    ImGui::PushID(this);
    if (ImGui::TreeNode("Camera")) {
        ImGui::Checkbox("Perspective camera",&perspective);
        if (perspective){
            ImGui::DragFloat("FovY",&fovY,1.0f,1,179);
            ImGui::DragFloat("Near-plane",&nearPlane,1.0f,0.01,10000);
            ImGui::DragFloat("Far-plane",&farPlane,1.0f,0.01,10000);
        } else {
            ImGui::DragFloat("Left",&size,1.0f,0.01,10000);
            ImGui::DragFloat("Near-plane",&nearPlane,1.0f,0.01,10000);
            ImGui::DragFloat("Far-plane",&farPlane,1.0f,0.01,10000);
        }
        ImGui::Checkbox("Clear ColorBuffer",&clearColorBuffer);
        ImGui::Checkbox("Clear DepthBuffer",&clearDepthBuffer);
        ImGui::ColorEdit4("Clear color",&clearColor.r);
        ImGui::TreePop();
    }
    ImGui::PopID();
}

Ray Camera::screenPointToRay(glm::vec2 p) {
    auto size = sre::Renderer::instance->getDrawableSize();
    glm::vec3 screenPoint1 = glm::vec3(p.x, size.y-p.y, 0.f);
    glm::vec3 screenPoint2 = glm::vec3(p.x, size.y-p.y, 1.f);

    glm::vec4 viewPort = glm::vec4(0.f,0.f,size.x,size.y);
    glm::vec3 eyePoint = glm::unProject(screenPoint1, camera.getViewTransform(), camera.getProjectionTransform(size), viewPort);
    glm::vec3 atPoint = glm::unProject(screenPoint2, camera.getViewTransform(), camera.getProjectionTransform(size), viewPort);

    Ray ray = {eyePoint, glm::normalize(atPoint - eyePoint)};
    return ray;
}

void Camera::moveHandCursor(glm::vec3 front, std::shared_ptr<GameObject> hand)
{
    glm::vec3 handCursorPosition = transform->position+glm::normalize(front)*0.75f;

    hand->getComponent<Transform>()->position = handCursorPosition;
    hand->getComponent<Transform>()->position.y -= 0.25f;
    //hand->getComponent<Transform>()->position.x -= 0.5f;

    hand->getComponent<Transform>()->lookAt(hand->getComponent<Transform>()->position + front, glm::vec3(0, 1, 0));
    hand->getComponent<Transform>()->rotation = transform->rotation;
}

void Camera::moveTowerCursor(glm::vec3 front, std::shared_ptr<GameObject> tower)
{
    if(this->getGameObject()->getComponent<PersonController>()->targetBlock != nullptr)
        return;

    glm::vec3 towerCursorPosition = calcTowerCursorPosition(front);

    tower->getComponent<Transform>()->position = towerCursorPosition;
    tower->getComponent<Transform>()->lookAt(camera.getPosition(), glm::vec3(0, 1, 0));
}

//http://www.3dkingdoms.com/weekly/weekly.php?a=3
void Camera::simpleRayCast(glm::vec3 front, std::shared_ptr<GameObject> tower, std::vector<std::shared_ptr<GameObject>> gameObjects)
{
    glm::vec3 towerCursorPosition = calcTowerCursorPosition(front);
    resetTowerCursor(tower, towerCursorPosition);

    bool foundIntersect = false;
    double currentDistance = 90000.0f;
    shared_ptr<GameObject> block = nullptr;

    /*vector<shared_ptr<GameObject>> closeBlocks;

    for(auto gameObject : gameObjects)
    {
        if(gameObject->getComponent<WorldObject>() == nullptr)
            continue;

        auto blockPosition = gameObject->getComponent<Transform>()->position;
        double distance = calcDistance(towerCursorPosition, blockPosition);

        if(distance < 1.5)
        {
            currentDistance = distance;
            closeBlocks.push_back(gameObject);
        }
    }
    cout << "CloseBlocksAmount: " << closeBlocks.size() << endl;*/

    for(auto gameObject : gameObjects)
    {
        if(gameObject->getComponent<WorldObject>() == nullptr)
            continue;

        auto blockPosition = gameObject->getComponent<Transform>()->position;
        glm::vec3 Hit;
        bool intersect = CheckIntersection(blockPosition, towerCursorPosition, Hit);
        if(!intersect)
            continue;

        foundIntersect = true;

        double distance = calcDistance(this->transform->position, blockPosition);

        if(block == nullptr)
        {
            currentDistance = distance;
            block = gameObject;
            cout << "assigned block: " << endl;
            continue;
        }

        if(distance < currentDistance)
        {
            currentDistance = distance;
            block = gameObject;
            cout << "assigned block: " << endl;
        }
    }

    if(!foundIntersect)
    {
        resetTowerCursor(tower, towerCursorPosition);
    }
    else
    {
        tower->getComponent<Transform>()->position = block->getComponent<Transform>()->position;
        tower->getComponent<Transform>()->position.y = tower->getComponent<Transform>()->position.y + 1;

        tower->getComponent<Transform>()->rotation = block->getComponent<Transform>()->rotation;
        this->getGameObject()->getComponent<PersonController>()->targetBlock = block;

        if(checkBuildableStatus(block))
        {
            materials[0]->setColor(colors[1]);

            this->getGameObject()->getComponent<PersonController>()->allowedToBuild = true;
        }
        else
        {
            materials[0]->setColor(colors[0]);
        }
        tower->getComponent<ModelRenderer>()->setMaterials(materials);
    }
}

int inline Camera::GetIntersection( float fDst1, float fDst2, glm::vec3 P1, glm::vec3 P2, glm::vec3 &Hit) {
    if ( (fDst1 * fDst2) >= 0.0f) return 0;
    if ( fDst1 == fDst2) return 0;
    Hit = P1 + (P2-P1) * ( -fDst1/(fDst2-fDst1) );
    return 1;
}

int inline Camera::InBox( glm::vec3 Hit, glm::vec3 B1, glm::vec3 B2, const int Axis) {
    if ( Axis==1 && Hit.z > B1.z && Hit.z < B2.z && Hit.y > B1.y && Hit.y < B2.y) return 1;
    if ( Axis==2 && Hit.z > B1.z && Hit.z < B2.z && Hit.x > B1.x && Hit.x < B2.x) return 1;
    if ( Axis==3 && Hit.x > B1.x && Hit.x < B2.x && Hit.y > B1.y && Hit.y < B2.y) return 1;
    return 0;
}

// returns true if line (L1, L2) intersects with the box (B1, B2)
// returns intersection point in Hit
int Camera::CheckLineBox( glm::vec3 B1, glm::vec3 B2, glm::vec3 L1, glm::vec3 L2, glm::vec3 &Hit)
{
    if (L2.x < B1.x && L1.x < B1.x) return false;
    if (L2.x > B2.x && L1.x > B2.x) return false;
    if (L2.y < B1.y && L1.y < B1.y) return false;
    if (L2.y > B2.y && L1.y > B2.y) return false;
    if (L2.z < B1.z && L1.z < B1.z) return false;
    if (L2.z > B2.z && L1.z > B2.z) return false;
    if (L1.x > B1.x && L1.x < B2.x &&
        L1.y > B1.y && L1.y < B2.y &&
        L1.z > B1.z && L1.z < B2.z)
    {Hit = L1;
        return true;}
    if ( (GetIntersection( L1.x-B1.x, L2.x-B1.x, L1, L2, Hit) && InBox( Hit, B1, B2, 1 ))
         || (GetIntersection( L1.y-B1.y, L2.y-B1.y, L1, L2, Hit) && InBox( Hit, B1, B2, 2 ))
         || (GetIntersection( L1.z-B1.z, L2.z-B1.z, L1, L2, Hit) && InBox( Hit, B1, B2, 3 ))
         || (GetIntersection( L1.x-B2.x, L2.x-B2.x, L1, L2, Hit) && InBox( Hit, B1, B2, 1 ))
         || (GetIntersection( L1.y-B2.y, L2.y-B2.y, L1, L2, Hit) && InBox( Hit, B1, B2, 2 ))
         || (GetIntersection( L1.z-B2.z, L2.z-B2.z, L1, L2, Hit) && InBox( Hit, B1, B2, 3 )))
        return true;

    return false;
}

int Camera::CheckIntersection(glm::vec3 blockPosition, glm::vec3 towerCursorPosition, glm::vec3 &Hit)
{
    glm::vec3 b1 = glm::vec3( blockPosition.x -0.5f, blockPosition.y -0.5f, blockPosition.z-0.5f);
    glm::vec3 b2 = glm::vec3( blockPosition.x + 0.5f, blockPosition.y + 0.5f, blockPosition.z+ 0.5f);
    return CheckLineBox( b1, b2, this->transform->position, towerCursorPosition, Hit);
}

void Camera::resetTowerCursor(std::shared_ptr<GameObject> tower, glm::vec3 position) {
    this->getGameObject()->getComponent<PersonController>()->allowedToBuild = false;
    this->getGameObject()->getComponent<PersonController>()->targetBlock = nullptr;
    materials[0]->setColor(colors[0]);
    tower->getComponent<ModelRenderer>()->setMaterials(materials);
    /*tower->getComponent<Transform>()->position = position;
    tower->getComponent<Transform>()->lookAt(camera.getPosition(), glm::vec3(0, 1, 0));*/
}

glm::vec3 Camera::calcTowerCursorPosition(glm::vec3 front) {
    glm::vec3 towerCursorPosition = transform->position+glm::normalize(front)*5.0f;
    return towerCursorPosition;
}

double Camera::calcDistance(glm::vec3 pos1, glm::vec3 pos2) {
    float x1 = pos1.x;
    float y1 = pos1.y;
    float z1 = pos1.z;

    float x2 = pos2.x;
    float y2 = pos2.y;
    float z2 = pos2.z;
    double distance = sqrt( pow(x1 - x2, 2.0) + pow(y1 - y2, 2.0) + pow(z1 - z2, 2.0));
    return distance;
}

bool Camera::checkBuildableStatus(shared_ptr<GameObject> block) {
    if(!block->getComponent<WorldObject>()->getBuildableStatus())
        return false;

    bool canAfford = GameManager::getInstance().getScore() >= GameManager::getInstance().selectedTower->getBuildCost();
    if(!canAfford)
        return false;

    return true;
}

sre::Camera Camera::getCamera() {
    return camera;
}

void Camera::update(float deltaTime)
{

}


