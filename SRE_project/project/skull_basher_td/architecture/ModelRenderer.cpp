//
// Created by Morten Nobel Jørgensen on 2018-11-06.
//

#include "ModelRenderer.hpp"
#include <imgui.h>
#include "Transform.hpp"
#include "GameObject.hpp"

using namespace sre;

ModelRenderer::ModelRenderer(GameObject* gameObject)
: Component(gameObject) {
    std::vector<std::shared_ptr<Material>> materials;
    materials.push_back(sre::Shader::getStandardBlinnPhong()->createMaterial());
    static auto sharedMeshCube = Mesh::create().withCube().build();
    model = Model::create().withMesh(sharedMeshCube).withMaterials(materials).build();
}

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
    if(!active)
        return;
    renderPass->draw(model->getMesh(), gameObject->getComponent<Transform>()->globalTransform(), model->getMaterials());
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

ModelRenderer::~ModelRenderer() {
}
