//
// Created by Morten Nobel Jørgensen on 2018-11-08.
//

#pragma once

#include <sre/Mesh.hpp>
#include <sre/Camera.hpp>
#include "GameObject.hpp"
#include <sre/Material.hpp>
#include "Transform.hpp"
#include "Renderable.hpp"
#include "Ray.hpp"

class Camera : public Component {
public:
    explicit Camera(GameObject * gameObject);
    ~Camera() override = default;

    void bind();

    //update the camera?
    void update(float deltaTime);

    bool perspective = true;
    float fovY = 45;
    float size;
    float nearPlane = 0.1;
    float farPlane = 100;
    bool clearColorBuffer = true;
    bool clearDepthBuffer = true;

    sre::Color clearColor = sre::Color(0,0,0,1);

    bool debugGui=true;

    void debugGUI() override ;

    Ray screenPointToRay(glm::vec2 point);

    sre::Camera getCamera();
    void simpleRayCast(glm::vec3 front, std::shared_ptr<GameObject> tower, std::vector<std::shared_ptr<GameObject>> gameObjects);
    void moveTowerCursor(glm::vec3 front, std::shared_ptr<GameObject> tower);
    int inline GetIntersection( float fDst1, float fDst2, glm::vec3 P1, glm::vec3 P2, glm::vec3 &Hit);
    int inline InBox( glm::vec3 Hit, glm::vec3 B1, glm::vec3 B2, const int Axis) ;
    int CheckLineBox( glm::vec3 B1, glm::vec3 B2, glm::vec3 L1, glm::vec3 L2, glm::vec3 &Hit);
    int CheckIntersection(glm::vec3 blockPosition, glm::vec3 towerCursorPosition, glm::vec3 &Hit);
    double calcDistance(glm::vec3 pos1, glm::vec3 pos2);
    bool checkBuildableStatus(std::shared_ptr<GameObject> block);
    void resetTowerCursor(std::shared_ptr<GameObject> tower, glm::vec3 position);
    glm::vec3 calcTowerCursorPosition(glm::vec3 front);
private:
    Transform* transform;
    sre::Camera camera;
    friend class Engine;
    friend class Scene;
    std::vector<sre::Color> colors;
    std::vector<std::shared_ptr<sre::Material>> materials;
};
