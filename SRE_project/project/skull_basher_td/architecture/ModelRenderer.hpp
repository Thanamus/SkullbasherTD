//
// Created by Morten Nobel Jørgensen on 2018-11-06.
//


#pragma once

#include "Model.hpp"
#include "Component.hpp"
#include "GameObject.hpp"
#include "Renderable.hpp"

class ModelRenderer : public Component, public Renderable {
public:
    explicit ModelRenderer(GameObject* gameObject);
    ~ModelRenderer() override;

    void setModel(std::shared_ptr<Model> model);
    std::shared_ptr<Model> getModel();

    void setMesh(std::shared_ptr<sre::Mesh> mesh);
    std::shared_ptr<sre::Mesh> getMesh();

    void setMaterials(std::vector<std::shared_ptr<sre::Material>> materials);
    std::vector<std::shared_ptr<sre::Material>> getMaterials();

    void draw(sre::RenderPass* renderPass) override;

    void debugGUI() override ;

private:
    std::shared_ptr<Model> model;
};
