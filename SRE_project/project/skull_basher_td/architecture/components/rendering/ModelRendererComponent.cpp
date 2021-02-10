//
// Created by Morten Nobel Jørgensen on 2018-11-06.
//

#include "ModelRendererComponent.hpp"
#include <imgui.h>
#include "../TransformComponent.hpp"

using namespace sre;

ModelRendererComponent::ModelRendererComponent(GameObject* gameObject)
: Component(gameObject) {
    // init with a standard cube!
    std::vector<std::shared_ptr<Material>> materials;
    materials.push_back(sre::Shader::getStandardBlinnPhong()->createMaterial());
    static auto sharedMeshCube = Mesh::create().withCube().build();
    model = Model::create().withMesh(sharedMeshCube).withMaterials(materials).build();
}

void ModelRendererComponent::setModel(std::shared_ptr<Model> model) {
    ModelRendererComponent::model = std::move(model);
}

std::shared_ptr<Model> ModelRendererComponent::getModel() {
    return model;
}


// kept for utility purposes
void ModelRendererComponent::setMesh(std::shared_ptr<sre::Mesh> mesh) {
    model->setMesh(mesh);
}
std::shared_ptr<sre::Mesh> ModelRendererComponent::getMesh(){
    return model->getMesh();
}

std::vector<std::shared_ptr<sre::Material>> ModelRendererComponent::getMaterials() {
    return model->getMaterials();
}
void ModelRendererComponent::setMaterials(std::vector<std::shared_ptr<sre::Material>> materials) {
    model->setMaterials(materials);
}

// draws model at location specified by transform
void ModelRendererComponent::draw(sre::RenderPass* renderPass) {
    if(!active)
        return;
    auto transformComp = gameObject->getComponent<TransformComponent>();
    glm::mat4 transform = (transformComp) ? transformComp->globalTransform() : model->getTransform();

    renderPass->draw(model->getMesh(), transform, model->getMaterials());
}

void ModelRendererComponent::debugGUI() {
    if (ImGui::TreeNode("ModelRendererComponent")) {
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

ModelRendererComponent::~ModelRendererComponent() {
    model.reset();
}
