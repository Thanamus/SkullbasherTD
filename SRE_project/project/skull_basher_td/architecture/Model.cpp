//
// Created by kb on 07/12/2020.
//

#include "Model.hpp"

Model::ModelBuilder &Model::ModelBuilder::withName(std::string name)
{
    this->name = name;
    return *this;
}

Model::ModelBuilder &Model::ModelBuilder::withMaterials(std::vector<std::shared_ptr<sre::Material>> materials)
{
    this->materials.clear();
    this->materials = materials;
    return *this;
}

Model::ModelBuilder &Model::ModelBuilder::withTransform(glm::mat4 transform)
{
    this->transform = transform;
    return *this;
}

Model::ModelBuilder &Model::ModelBuilder::withMesh(std::shared_ptr<sre::Mesh> mesh)
{
    this->mesh = mesh;
    return *this;
}

Model::ModelBuilder &Model::ModelBuilder::withOBJ(std::string file)
{
    auto pos = file.find_last_of(PATH_SEPARATOR) + 1;
    auto path = file.substr(0, pos);
    auto filename = file.substr(pos);
    this->mesh = sre::ModelImporter::importObj(path, filename, materials);
    std::cout << "deb2 " << materials.size() << std::endl;
    for (auto m : materials)
    {
        std::cout << m->getName() << std::endl;
    }
    return *this;
}

std::shared_ptr<Model> Model::ModelBuilder::build()
{
    if (name.length() == 0)
    {
        name = "Unnamed model";
    }
    auto model = new Model(name, mesh, materials, transform);
    return std::shared_ptr<Model>(model);
}

Model::ModelBuilder Model::create()
{
    return Model::ModelBuilder();
}

Model::Model(std::string name, std::shared_ptr<sre::Mesh> mesh, std::vector<::std::shared_ptr<sre::Material>> materials,
             glm::mat4 transform = glm::mat4(0))
{
    this->name = name;
    this->mesh = mesh;
    this->materials = materials;
    this->transform = transform;
}

const std::string &Model::getName() const
{
    return name;
}

void Model::setName(const std::string &name)
{
    this->name = name;
}

std::shared_ptr<sre::Mesh> &Model::getMesh()
{
    return mesh;
}

void Model::setMesh(const std::shared_ptr<sre::Mesh> &mesh)
{
    this->mesh = mesh;
}

const std::vector<std::shared_ptr<sre::Material>> &Model::getMaterials()
{
    return materials;
}

void Model::setMaterials(const std::vector<std::shared_ptr<sre::Material>> &materials)
{
    this->materials = materials;
}

const glm::mat4 &Model::getTransform() const
{
    return transform;
}

void Model::setTransform(const glm::mat4 &transform)
{
    this->transform = transform;
}
