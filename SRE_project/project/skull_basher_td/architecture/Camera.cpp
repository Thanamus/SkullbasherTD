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



using namespace std;
using namespace glm;
#include <functional>   // std::ref

Camera::Camera(GameObject * gameObject)
:Component(gameObject)
{
    transform = gameObject->getComponent<Transform>().get();
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

//http://www.3dkingdoms.com/weekly/weekly.php?a=3
void Camera::TestRay(glm::vec3 front, std::shared_ptr<GameObject> rayTestedCube, std::shared_ptr<GameObject> tower, std::vector<std::shared_ptr<GameObject>> gameObjects)
{
    auto size = sre::Renderer::instance->getDrawableSize();
    auto ray = screenPointToRay(glm::vec2(size.x / 2, size.y / 2));

    glm::vec3 tDirection = transform->rotation * front;

    glm::vec3 target = transform->position + tDirection * 10.0f;

    glm::vec3 test = transform->position+glm::normalize(front)*5.0f;

    rayTestedCube->getComponent<Transform>()->position = test;
    rayTestedCube->getComponent<Transform>()->lookAt(camera.getPosition(), glm::vec3(0, 1, 0));

    vector<sre::Color> colors = {
            {1, 0, 0, 0.5},
            {0, 1, 0, 0.5},
            {0, 0, 1, 0.5},
            {1, 1, 1, 0.5}};
    vector<std::shared_ptr<sre::Material>> materials = {sre::Shader::getUnlit()->createMaterial()};

    bool foundIntersect = false;
    bool first = false;
    float distance = 0.0f;
    for(auto gameObject : gameObjects)
    {
        if(gameObject->getComponent<WorldObject>() == nullptr)
            continue;

        auto position = gameObject->getComponent<Transform>()->position;
        glm::vec3 b1 = glm::vec3( position.x -0.5f, position.y -0.5f, position.z-0.5f);
        glm::vec3 b2 = glm::vec3( position.x + 0.5f, position.y + 0.5f, position.z+ 0.5f);
        glm::vec3 Hit;
        bool intersect = CheckLineBox( b1, b2, this->transform->position, test, Hit);
        if(!intersect)
            continue;

        foundIntersect = true;

        float currentDistance = sqrt(pow(this->transform->position.x, tower->getComponent<Transform>()->position.x) + pow(this->transform->position.y, tower->getComponent<Transform>()->position.y));

        if(!first)
        {
            distance = currentDistance;
            first = true;
            tower->getComponent<Transform>()->position = position;
            tower->getComponent<Transform>()->position.y = rayTestedCube->getComponent<Transform>()->position.y + 1;
            tower->getComponent<Transform>()->rotation = gameObject->getComponent<Transform>()->rotation;
            continue;
        }

        if(currentDistance < distance)
        {
            distance = currentDistance;
            tower->getComponent<Transform>()->position = position;
            tower->getComponent<Transform>()->position.y = rayTestedCube->getComponent<Transform>()->position.y + 1;
            tower->getComponent<Transform>()->rotation = gameObject->getComponent<Transform>()->rotation;
        }

        if(gameObject->getComponent<WorldObject>()->getBuildableStatus())
            materials[0]->setColor(colors[1]);
        else
            materials[0]->setColor(colors[0]);
        tower->getComponent<ModelRenderer>()->setMaterials(materials);
    }

    if(!foundIntersect)
    {
        materials[0]->setColor(colors[0]);
        tower->getComponent<ModelRenderer>()->setMaterials(materials);
        tower->getComponent<Transform>()->position = test;
        tower->getComponent<Transform>()->lookAt(camera.getPosition(), glm::vec3(0, 1, 0));
    }
}

bool Camera::AabbContainsSegment (float x1, float y1, float x2, float y2, float minX, float minY, float maxX, float maxY) {
    // Completely outside.
    if ((x1 <= minX && x2 <= minX) || (y1 <= minY && y2 <= minY) || (x1 >= maxX && x2 >= maxX) || (y1 >= maxY && y2 >= maxY))
        return false;

    float m = (y2 - y1) / (x2 - x1);

    float y = m * (minX - x1) + y1;
    if (y > minY && y < maxY) return true;

    y = m * (maxX - x1) + y1;
    if (y > minY && y < maxY) return true;

    float x = (minY - y1) / m + x1;
    if (x > minX && x < maxX) return true;

    x = (maxY - y1) / m + x1;
    if (x > minX && x < maxX) return true;

    return false;
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

sre::Camera Camera::getCamera() {
    return camera;
}

void Camera::update(float deltaTime)
{

}


