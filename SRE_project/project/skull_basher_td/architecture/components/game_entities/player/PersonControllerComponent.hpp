//
// Created by Morten Nobel-Jørgensen on 29/09/2017.
//

#include <SDL_events.h>
#include "CameraComponent.hpp"

#include "../../Component.hpp"
#include "../../TransformComponent.hpp"
#include "../../../scenes/GameObject.hpp"

// #include "./collisions/CollisionHandler.hpp"

#include <chrono>

class PlayerCollisionController;


// class PersonControllerComponent : public Component, public CollisionHandler {
class PersonControllerComponent : public Component {
public:
    explicit PersonControllerComponent(GameObject* GameObject);
    virtual ~PersonControllerComponent();

    void debugGUI() override ;

    void update(float deltaTime);
    void updateInput(float deltaTime);
    void updateVectors();
    void onKey(SDL_Event &event);
    void onMouse(SDL_Event &event);

    void setInitialPosition(glm::vec2 position, float rotation);

    // float rotation = 0.f;
    float rotation = -90.f;
    glm::vec3 position;
    bool quit = false;
    std::shared_ptr<CameraComponent> camera;
    std::shared_ptr<GameObject> hand;
    std::shared_ptr<GameObject> tower;
    std::shared_ptr<GameObject> targetBlock;
    bool allowedToBuild = false;
    void updateHandModel(std::string model);
    int getReloadLockoutMillisec() const;
    friend class PlayerCollisionHandlerComponent;


    const std::map<std::string, std::shared_ptr<Model>> &getHandModels() const;

    const bool isGrounded() const;

    bool& getGrounded();

    void setIsGrounded(bool isGrounded);

    void setHandModels(const std::map<std::string, std::shared_ptr<Model>> &handModels);

private:

    void fireProjectile();
    std::map<std::string, std::shared_ptr<Model>> handModels;
    std::shared_ptr<Model> arrowModel;

    glm::vec3 world_up = glm::vec3(0, 1, 0);
    glm::vec3 camera_front;
    glm::vec3 camera_dir;
    glm::vec3 camera_right;
    glm::vec3 camera_fwd;

    glm::vec3 original_camera_dir;

    // float movespeed = 8.f;
    float currentMovespeed = 5.f;
    const float slowMovespeed = 2.5f;
    const float normalMovespeed = 5.f;
    // float sensitivity = 1.7f;
    float sensitivity = 35.0f;

    std::chrono::steady_clock::time_point start_reload_lockout;
    std::chrono::steady_clock::time_point kickOffTime_reload;
    int reload_lockout_millisec = 2500;

    bool shootable = true;

    float jumpHeight = 7.f;

    // // Footstep sound timing
    // std::chrono::steady_clock::time_point start_footstep_lockout;
    // int footstep_lockout_millisec = 300;

    // input stuff
    bool key_fwd = false;
    bool key_bwd = false;
    bool key_left = false;
    bool key_right = false;

    //turn these off for the real game
    bool key_flyUp = false;
    bool key_flyDown = false;

    bool key_jump = false;

    // float mouse_offset = 0.f;
    glm::vec2 mouse_offset = {0.0f, 0.0f};
    float pitch = 0;
    float pitch_limit = 30.f;

    bool grounded = false;
};
