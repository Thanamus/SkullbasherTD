//
// Created by Morten Nobel Jørgensen on 2018-11-08.
//

#include "Camera.hpp"
#include "sre/Renderer.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"
#include "glm/gtx/rotate_vector.hpp"

using namespace std;
using namespace glm;

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

void Camera::TestRay(glm::vec3 front, std::shared_ptr<GameObject> rayTestedCube)
{
    auto size = sre::Renderer::instance->getDrawableSize();
    auto ray = screenPointToRay(glm::vec2(size.x / 2, size.y / 2));

    glm::vec3 tDirection = transform->rotation * front;
    //glm::vec3 tDirection = transform->rotation * glm::vec3 (0,-1,0);

    glm::vec3 target = transform->position + tDirection * 10.0f;
    //ray.point.z = ray.point.z + 15;

    /*glm::vec3 cameraFront = glm::vec3(cos(glm::radians(transform->rotation.x)),0,glm::sin(glm::radians(transform->rotation.z)));

    glm::vec3 direction;
    direction.x = glm::cos(radians(yaw)) * cos(glm::radians(pitch));
    direction.y = glm::sin(glm::radians(pitch));
    direction.z = glm::sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);*/

    //test.z += 10;

    glm::vec3 test = transform->position+glm::normalize(front)*5.0f;

    rayTestedCube->getComponent<Transform>()->position = test;
}


sre::Camera Camera::getCamera() {
    return camera;
}

void Camera::update(float deltaTime)
{

}


