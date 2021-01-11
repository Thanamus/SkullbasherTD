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
    void TestRay(glm::vec3 front, std::shared_ptr<GameObject> rayTestedCube, std::shared_ptr<GameObject> tower, std::vector<std::shared_ptr<GameObject>> gameObjects);
    bool Camera::AabbContainsSegment (float x1, float y1, float x2, float y2, float minX, float minY, float maxX, float maxY);
    int inline Camera::GetIntersection( float fDst1, float fDst2, glm::vec3 P1, glm::vec3 P2, glm::vec3 &Hit);
    int inline Camera::InBox( glm::vec3 Hit, glm::vec3 B1, glm::vec3 B2, const int Axis) ;
    int Camera::CheckLineBox( glm::vec3 B1, glm::vec3 B2, glm::vec3 L1, glm::vec3 L2, glm::vec3 &Hit) ;
private:
    Transform* transform;
    sre::Camera camera;
    friend class Engine;
    friend class Scene;
};