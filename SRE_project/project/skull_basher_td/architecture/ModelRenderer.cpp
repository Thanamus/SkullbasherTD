//
// Created by Morten Nobel Jørgensen on 2018-11-06.
//

#include "ModelRenderer.hpp"

#include <sre/Renderer.hpp>
#include <imgui.h>
#include "ModelRenderer.hpp"
#include "Animator.hpp"

using namespace sre;

ModelRenderer::ModelRenderer(GameObject* gameObject)
: Component(gameObject), transform(gameObject->getComponent<Transform>().get()), animator(gameObject->getComponent<Animator>().get()) {
    std::vector<std::shared_ptr<Material>> materials;
    materials.push_back(sre::Shader::getStandardBlinnPhong()->createMaterial());
    static auto sharedMeshCube = Mesh::create().withCube().build();
    model = Model::create().withMesh(sharedMeshCube).withMaterials(materials).build();
}

ModelRenderer::~ModelRenderer() = default;

void ModelRenderer::setModel(std::shared_ptr<Model> model) {
    ModelRenderer::model = std::move(model);
}

std::shared_ptr<Model> ModelRenderer::getModel() {
    return model;
}


// kept for utility purposes
void ModelRenderer::setMesh(std::shared_ptr<sre::Mesh> mesh) {
    model->setMesh(mesh);
}
std::shared_ptr<sre::Mesh> ModelRenderer::getMesh(){
    return model->getMesh();
}

std::vector<std::shared_ptr<sre::Material>> ModelRenderer::getMaterials() {
    return model->getMaterials();
}
void ModelRenderer::setMaterials(std::vector<std::shared_ptr<sre::Material>> materials) {
    model->setMaterials(materials);
}

void ModelRenderer::draw(sre::RenderPass* renderPass) {
    auto compositeTransform = model->getTransform();
    if(transform)
        compositeTransform *= transform->globalTransform();
    if(animator)
        compositeTransform *= animator->getSQTMatrix();

    renderPass->draw(model->getMesh(), compositeTransform, model->getMaterials());
}

void ModelRenderer::debugGUI() {
    if (ImGui::TreeNode("ModelRenderer")) {
        if (ImGui::TreeNode("Materials")) {
            for(const auto& material : getMaterials()) {
                ImGui::LabelText("Material", "%s", material->getName().c_str());
                auto color = material->getColor();
                if (ImGui::DragFloat4("Color", &(color.r))){
                    material->setColor(color);
                }
                auto specularity = material->getSpecularity();
                if (ImGui::ColorEdit3("Specularity Color", &(color.r))){
                    material->setSpecularity(specularity);
                }
                if (ImGui::DragFloat("Specularity", &(specularity.a))){
                    material->setSpecularity(specularity);
                }
                ImGui::TreePop();
            }
        }
        ImGui::TreePop();
    }
}

Transform *ModelRenderer::getTransform() const {
    return transform;
}

void ModelRenderer::setTransform(Transform *transform) {
    ModelRenderer::transform = transform;
}

Animator *ModelRenderer::getAnimator() const {
    return animator;
}

void ModelRenderer::setAnimator(Animator *animator) {
    ModelRenderer::animator = animator;
}

