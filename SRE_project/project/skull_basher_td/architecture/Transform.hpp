//
// Created by Morten Nobel Jørgensen on 2018-11-06.
//
#pragma once
#define GLM_ENABLE_EXPERIMENTAL
#include <sre/Mesh.hpp>
#include "glm/glm.hpp"
#include <string>
#include <vector>
#include "Component.hpp"
#include "GameObject.hpp"
#include "Animator.hpp"
#include "ModelRenderer.hpp"


class Transform : public Component {
public:
    explicit Transform(GameObject* gameObject);
    ~Transform();

    std::string name;

    glm::vec3 position = glm::vec3(0,0,0);
    // euler rotation in degrees (default value 0,0,0)
    glm::vec3 rotation = glm::vec3(0,0,0);
    glm::vec3 scale = glm::vec3(1,1,1);

    glm::mat4 localRotation() const;
    glm::mat4 localTransform() const;

    glm::vec3 globalRotation();
    glm::vec3 globalPosition();
    glm::mat4 globalTransform();

    std::shared_ptr<Animator> getAnimator() const;
    void setAnimator(std::shared_ptr<Animator> _animator);

    std::shared_ptr<ModelRenderer> getModelRenderer() const;
    void setModelRenderer(std::shared_ptr<ModelRenderer> _modelRenderer);


    void debugGUI() override;

//    Transform *getParent() const;
//
//    void setParent(Transform *_parent);
//    const std::vector<Transform*> & getChildren();

    void lookAt(glm::vec3 at,glm::vec3 up);
    void lookAt(Transform* at,glm::vec3 up);

private:
//    Transform * parent = nullptr;
//    // make it shared
//    std::vector<Transform*> children;
    glm::mat4 global = glm::mat4(1.);
    std::shared_ptr<ModelRenderer> modelRenderer;
    std::shared_ptr<Animator> animator;
};


