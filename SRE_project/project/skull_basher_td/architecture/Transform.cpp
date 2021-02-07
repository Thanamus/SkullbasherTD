//
// Created by Morten Nobel Jørgensen on 2018-11-06.
//

#include "Transform.hpp"

#include <sre/Renderer.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>
#include <algorithm>
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/euler_angles.hpp"
#include "glm/gtx/string_cast.hpp"

using namespace sre;

Transform::Transform(GameObject* gameObject)
        : Component(gameObject) {
    if(gameObject->getParent() && gameObject->getComponent<Transform>().get()) {
        setParent(gameObject->getComponent<Transform>().get());
    }

    // sets animator and renderer pointers (if found)
    setAnimator(gameObject->getComponent<Animator>());
    setModelRenderer(gameObject->getComponent<ModelRenderer>());
}

std::shared_ptr<ModelRenderer> Transform::getModelRenderer() const {
    return modelRenderer;
}

void Transform::setModelRenderer(std::shared_ptr<ModelRenderer> _modelRenderer) {
    Transform::modelRenderer = _modelRenderer;
}

std::shared_ptr<Animator> Transform::getAnimator() const {
    return animator;
}

void Transform::setAnimator(std::shared_ptr<Animator> _animator) {
    Transform::animator = _animator;
}


glm::mat4 Transform::localTransform() const {
    glm::mat4 translateMat = glm::translate(glm::mat4(1), position);

    glm::mat4 scaleMat = glm::scale(glm::mat4(1), scale);

    auto compositeTransform = translateMat*localRotation()*scaleMat;
    if(modelRenderer && modelRenderer->getModel())
        compositeTransform *= modelRenderer->getModel()->getTransform();
    if(animator)
        compositeTransform *= animator->getSQTMatrix();

    return compositeTransform;
}

glm::mat4 Transform::globalTransform() const {
    if (parent){
        return parent->globalTransform() * localTransform() ;
    }

    return localTransform();
}

void Transform::debugGUI() {
    if (ImGui::TreeNode("Transform")) {
        ImGui::DragFloat3("position", &(position.x));
        ImGui::DragFloat3("rotation", &(rotation.x));
        ImGui::DragFloat3("scale", &(scale.x));
        ImGui::TreePop();
    }
}

Transform *Transform::getParent() const {
    return parent;
}

void Transform::setParent(Transform *_parent) {
    if (Transform::parent != nullptr){
        auto& parentChildren = Transform::parent->children;
        auto pos = std::find(parentChildren.begin(), parentChildren.end(), this);
        if (pos != parentChildren.end()){
            parentChildren.erase(pos);
        }
    }
    Transform::parent = _parent;
    if (_parent != nullptr){
        _parent->children.push_back(this);
    }
}

const std::vector<Transform *> &Transform::getChildren() {
    return children;
}

void Transform::lookAt(glm::vec3 at,glm::vec3 up){
    auto lookAtMat = glm::lookAt(position, at, up);
    float rotXangle, rotYangle, rotZangle;
    // http://gamedev.stackexchange.com/a/112271
    rotXangle = atan2(-lookAtMat[2][1], lookAtMat[2][2]);
    auto cosYangle = (float)sqrt(pow(lookAtMat[0][0], 2) + pow(lookAtMat[1][0], 2));
    rotYangle = atan2(lookAtMat[2][0], cosYangle);
    float sinXangle = sin(rotXangle);
    float cosXangle = cos(rotXangle);
    rotZangle = atan2(cosXangle * lookAtMat[0][1] + sinXangle * lookAtMat[0][2], cosXangle * lookAtMat[1][1] + sinXangle * lookAtMat[1][2]);
    this->rotation = -glm::degrees(glm::vec3{rotXangle, rotYangle, rotZangle});
}

void Transform::lookAt(Transform* at,glm::vec3 up){
    lookAt(at->position, up);
}

glm::mat4 Transform::localRotation() const {
    glm::mat4 rotZ = glm::eulerAngleZ(glm::radians(rotation.z));
    glm::mat4 rotY = glm::eulerAngleY(glm::radians(rotation.y));
    glm::mat4 rotX = glm::eulerAngleX(glm::radians(rotation.x));
    return rotZ*rotY*rotX;
}

glm::vec3 Transform::globalPosition() const {
    // returns translation vector from the global matrix
    return glm::vec3(globalTransform()[3]);
}

Transform::~Transform() {
    children.clear();
    parent = nullptr;
    animator.reset();
    modelRenderer.reset();
}

