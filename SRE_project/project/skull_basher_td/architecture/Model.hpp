//
// Created by kb on 07/12/2020.
//

#ifndef SIMPLERENDERENGINEPROJECT_MODEL_H
#define SIMPLERENDERENGINEPROJECT_MODEL_H

#include <iostream>
#include <vector>
#include <fstream>

#include <sre/Material.hpp>
#include <sre/Mesh.hpp>
#include <sre/ModelImporter.hpp>


#ifndef PATH_SEPARATOR
#define PATH_SEPARATOR '\\'
#ifndef _WIN32
#undef PATH_SEPARATOR
#define PATH_SEPARATOR '/';
#endif
#endif

/**
 * Mostly a useful class that puts together mesh, materials and transform for ease of rendering. Also allows to have a
 * single "point of entry" in making models (rather than keeping track of meshes, materials, etc. separately).
 */
class Model {
private:
    std::string name;
    std::shared_ptr<sre::Mesh> mesh;
    std::vector<std::shared_ptr<sre::Material>> materials;
    glm::mat4 transform{};

    Model(std::string name, std::shared_ptr<sre::Mesh> mesh, std::vector<::std::shared_ptr<sre::Material>> materials, glm::mat4 transform);
public:
    /**
     * Builder pattern implementation for easo of use and future-proofing.
     */
    class ModelBuilder {
    public:
        ModelBuilder& withName(std::string name);
        ModelBuilder& withMaterials(std::vector<std::shared_ptr<sre::Material>> materials);
        ModelBuilder& withTransform(glm::mat4 transform);
        ModelBuilder& withMesh(std::shared_ptr<sre::Mesh> mesh);
        ModelBuilder& withOBJ(const std::string& file);
        std::shared_ptr<Model> build();

    private:
        std::string name;
        std::shared_ptr<sre::Mesh> mesh;
        std::vector<std::shared_ptr<sre::Material>> materials;
        glm::mat4 transform{};

        ModelBuilder() = default;
        ModelBuilder(const ModelBuilder&) = default;

        friend class Model;
    };

    static ModelBuilder create();

    const std::string &getName() const;
    void setName(const std::string &name);

    std::shared_ptr<sre::Mesh> &getMesh();
    void setMesh(const std::shared_ptr<sre::Mesh> &mesh);

    const std::vector<std::shared_ptr<sre::Material>> &getMaterials();
    void setMaterials(const std::vector<std::shared_ptr<sre::Material>> &materials);

    const glm::mat4 &getTransform() const;
    void setTransform(const glm::mat4 &transform);

    ~Model()= default;

};


#endif //SIMPLERENDERENGINEPROJECT_MODEL_H
