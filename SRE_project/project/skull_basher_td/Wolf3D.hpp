#pragma once

#include "sre/SDLRenderer.hpp"
#include "sre/Material.hpp"
#include "WorldMap.hpp"
#include "GuiManager.hpp"
#include "FirstPersonController.hpp"

class Wolf3D
{
public:
    enum VertexPos
    {
        BOT_L = 0,
        BOT_R,
        TOP_L,
        TOP_R
    };

    enum Plane
    {
        XY = 0,
        YZ,
        XZ
    };
    void ToggleLockMouse();

    // some useful public const regarding the tileset
    const short unsigned int texturesPerRow = 16;
    const short unsigned int textureVariations = 2; // how many variations of a texture; in this case just light/dark
    const short unsigned int rowOffset = 42;        // negative offset because the type 0 texture is on the 42nd row, and we need to go "down" the rows
    const short unsigned int columnOffset = 0;      // in this case we don't need it, since we start on the first column

    Wolf3D();

private:
    void init();
    void update(float deltaTime);
    void render();
    void addCube(std::vector<glm::vec3> &vertexPositions, std::vector<glm::vec4> &textureCoordinates, int x, int z, int type);
    std::map<VertexPos, glm::vec3> makeQuadVertexMap(float side, Plane plane, glm::vec3 axesDirs, int x, int y, int z);
    void renderDebugBricks(sre::RenderPass &renderPass);
    void renderCeiling(sre::RenderPass &renderPass);
    void renderFloor(sre::RenderPass &renderPass);
    sre::SDLRenderer r;
    sre::Camera camera;
    WorldMap map;
    std::shared_ptr<GuiManager> guiManager;
    std::shared_ptr<GameManager> gameManager;
    std::shared_ptr<sre::Mesh> walls;
    std::shared_ptr<sre::Material> wallMaterial;
    FirstPersonController fpsController;
    glm::vec4 floorColor;
    glm::vec4 ceilColor;
    bool debugBricks = true;
    bool lockRotation = false;
};
