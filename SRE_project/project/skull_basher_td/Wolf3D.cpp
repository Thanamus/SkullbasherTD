//
// Created by Morten Nobel-Jørgensen on 22/09/2017.
//

#include "Wolf3D.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>
#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>
#ifdef _DEBUG
#define DBG_NEW new (_NORMAL_BLOCK, __FILE__, __LINE__)
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

using namespace std;
using namespace sre;
using namespace glm;

Wolf3D::Wolf3D()
    : fpsController(&camera)
{
    r.init();
    init();

    // Enable mouse lock
    // SDL_SetWindowGrab(r.getSDLWindow(), SDL_TRUE);
    // SDL_SetRelativeMouseMode(SDL_TRUE);

    r.frameUpdate = [&](float deltaTime) {
        update(deltaTime);
    };
    r.frameRender = [&]() {
        render();
    };
    r.keyEvent = [&](SDL_Event &e) {
        fpsController.onKey(e);
        gameManager->onKey(e);
    };
    r.mouseEvent = [&](SDL_Event &e) {
        if (!lockRotation)
        {
            fpsController.onMouse(e);
        }
    };
    r.startEventLoop();
}

void Wolf3D::update(float deltaTime)
{
    fpsController.update(deltaTime);
    if (gameManager->quit)
        r.stopEventLoop();
}

void Wolf3D::render()
{
    auto renderPass = RenderPass::create()
                          .withCamera(camera)
                          .build();

    // renderPass.draw(walls, glm::mat4(1), wallMaterial);
    // renderFloor(renderPass);
    // renderCeiling(renderPass);

    if (debugBricks)
    {
        renderDebugBricks(renderPass);
    }


    guiManager->onGui();
    ImGui::End();
}

// Creates a textured cube with a side length of 1 (e.g. from -0.5 to 0.5).
// The cube must be centered at (x,0,z)
// The texturing
void Wolf3D::addCube(std::vector<glm::vec3> &vertexPositions, std::vector<glm::vec4> &textureCoordinates, int x, int z, int type)
{
    const float side = 1.f;
    std::map<VertexPos, glm::vec3> vertexes;

    // this part could be cleaned up, but I guess readability is adequate so for now I'll let it be

    //first XY face
    vertexes = Wolf3D::makeQuadVertexMap(side, XY, vec3(1, 1, 1), x, 0, z);
    vertexPositions.insert(vertexPositions.end(), {// first triangle
                                                   vertexes[BOT_L],
                                                   vertexes[BOT_R],
                                                   vertexes[TOP_L],
                                                   // second triangle
                                                   vertexes[TOP_R],
                                                   vertexes[TOP_L],
                                                   vertexes[BOT_R]});
    //first YZ face
    vertexes = Wolf3D::makeQuadVertexMap(side, YZ, vec3(1, 1, -1), x, 0, z);
    vertexPositions.insert(vertexPositions.end(), {// first triangle
                                                   vertexes[BOT_L],
                                                   vertexes[BOT_R],
                                                   vertexes[TOP_L],
                                                   // second triangle
                                                   vertexes[TOP_R],
                                                   vertexes[TOP_L],
                                                   vertexes[BOT_R]});
    //second XY face
    vertexes = Wolf3D::makeQuadVertexMap(side, XY, vec3(-1, 1, -1), x, 0, z);
    vertexPositions.insert(vertexPositions.end(), {// first triangle
                                                   vertexes[BOT_L],
                                                   vertexes[BOT_R],
                                                   vertexes[TOP_L],
                                                   // second triangle
                                                   vertexes[TOP_R],
                                                   vertexes[TOP_L],
                                                   vertexes[BOT_R]});
    //second YZ face
    vertexes = Wolf3D::makeQuadVertexMap(side, YZ, vec3(-1, 1, 1), x, 0, z);
    vertexPositions.insert(vertexPositions.end(), {// first triangle
                                                   vertexes[BOT_L],
                                                   vertexes[BOT_R],
                                                   vertexes[TOP_L],
                                                   // second triangle
                                                   vertexes[TOP_R],
                                                   vertexes[TOP_L],
                                                   vertexes[BOT_R]});

    glm::vec2 textureSize(2048, 4096);
    glm::vec2 tileSize(64, 64);
    glm::vec2 tileSizeWithBorder(65, 65);

    // Step 1: get the correct texture
    int textureRow = rowOffset - (type * textureVariations) / texturesPerRow; // find the correct row
    int textureColumn = type % (texturesPerRow / textureVariations);          // find the correct column, aka the column of the light variation

    // Step 2: get the general texture coordinates
    int textureXCoord = textureColumn * tileSizeWithBorder.x * textureVariations; // find x coordinate of light variation
    int textureYCoord = textureRow * tileSizeWithBorder.y;

    // Step 3: generate light/dark texture coordinates
    glm::vec2 minLight = vec2(textureXCoord, textureYCoord) / textureSize;
    glm::vec2 minDark = vec2(textureXCoord + (float)tileSizeWithBorder.x, textureYCoord) / textureSize;
    glm::vec2 maxLight = minLight + tileSize / textureSize;
    glm::vec2 maxDark = minDark + tileSize / textureSize;

    for (int i = 0; i < 4; i++)
    {
        // this is done with the assumption that faces belong to alternate planes (that's why I went for first XY / first YZ / etc.)
        glm::vec2 min = (i % 2 == 0) ? minLight : minDark;
        glm::vec2 max = (i % 2 == 0) ? maxLight : maxDark;
        textureCoordinates.insert(
            textureCoordinates.end(), {glm::vec4(min.x, min.y, 0, 0), glm::vec4(max.x, min.y, 0, 0), glm::vec4(min.x, max.y, 0, 0),
                                       glm::vec4(max.x, max.y, 0, 0), glm::vec4(min.x, max.y, 0, 0), glm::vec4(max.x, min.y, 0, 0)});
    }
}

/**
 * Generates a map of vertexes to generate a quad (as a cube face) in a certain plane. Strict enforcing of parameter constraints will be implemented at a later time, if deemed adequate.
 * 
 * @param  {float} side                   : side of the cube
 * @param  {Wolf3D::Plane} plane          : plane in which the face should be visible
 * @param  {vec3} axesDirs(x,y,z)         : specifies orientation relative the coordinate system (needed since only CCW triangles are rendered, makes sure that vertexes are always created in the right order); each value should be 1, or -1; a value of 0 should only be used for the axis perpendicular to the desired plane to make the quad "purely 2D" (instead of centering it for a cube)
 * @param  {int} x                        : x offset from origin
 * @param  {int} y                        : y offset from origin
 * @param  {int} z                        : z offset from origin
 * @return {map<Wolf3D::VertexPos,vec3>}  : a map of 4 vertexes (bottom left, bottom right, top left, top right) that define a quad in a certain plane as a face of a cube centered on (x, y, z);
 */
map<Wolf3D::VertexPos, vec3> Wolf3D::makeQuadVertexMap(float side, Wolf3D::Plane plane, vec3 axesDirs, int x, int y, int z)
{
    float dist = side / 2;
    vec3 translate = vec3(x, y, z);
    std::map<Wolf3D::VertexPos, vec3> vertexes;
    switch (plane)
    {
    case XY:
        vertexes.insert(make_pair(BOT_L, translate + vec3(-dist * axesDirs.x, -dist * axesDirs.y, dist * axesDirs.z)));
        vertexes.insert(make_pair(BOT_R, translate + vec3(dist * axesDirs.x, -dist * axesDirs.y, dist * axesDirs.z)));
        vertexes.insert(make_pair(TOP_L, translate + vec3(-dist * axesDirs.x, dist * axesDirs.y, dist * axesDirs.z)));
        vertexes.insert(make_pair(TOP_R, translate + vec3(dist * axesDirs.x, dist * axesDirs.y, dist * axesDirs.z)));
        break;
    case YZ:
        vertexes.insert(make_pair(BOT_L, translate + vec3(dist * axesDirs.x, -dist * axesDirs.y, -dist * axesDirs.z)));
        vertexes.insert(make_pair(BOT_R, translate + vec3(dist * axesDirs.x, -dist * axesDirs.y, dist * axesDirs.z)));
        vertexes.insert(make_pair(TOP_L, translate + vec3(dist * axesDirs.x, dist * axesDirs.y, -dist * axesDirs.z)));
        vertexes.insert(make_pair(TOP_R, translate + vec3(dist * axesDirs.x, dist * axesDirs.y, dist * axesDirs.z)));
        break;
    case XZ:
        vertexes.insert(make_pair(BOT_L, translate + vec3(-dist * axesDirs.x, dist * axesDirs.y, -dist * axesDirs.z)));
        vertexes.insert(make_pair(BOT_R, translate + vec3(-dist * axesDirs.x, dist * axesDirs.y, dist * axesDirs.z)));
        vertexes.insert(make_pair(TOP_L, translate + vec3(dist * axesDirs.x, dist * axesDirs.y, -dist * axesDirs.z)));
        vertexes.insert(make_pair(TOP_R, translate + vec3(dist * axesDirs.x, dist * axesDirs.y, dist * axesDirs.z)));
        break;
    default:
        break;
    }
    return vertexes;
}

void Wolf3D::init()
{
    wallMaterial = Shader::getUnlit()->createMaterial();
    auto texture = Texture::create().withFile("level0.png").withGenerateMipmaps(false).withFilterSampling(false).build();
    wallMaterial->setTexture(texture);

    map.loadMap("level0.json");

    std::vector<glm::vec3> vertexPositions;
    std::vector<glm::vec4> textureCoordinates;

    for (int x = 0; x < map.getWidth(); x++)
    {
        for (int y = 0; y < map.getHeight(); y++)
        {
            int field = map.getTile(x, y);
            if (field != -1)
            {
                // addCube(vertexPositions, textureCoordinates, x, y, field);
            }
        }
    }

    fpsController.setInitialPosition(map.getStartingPosition(), map.getStartingRotation());

    walls = Mesh::create()
                .withPositions(vertexPositions)
                .withUVs(textureCoordinates)
                .build();

    floorColor = map.getFloorColor();
    ceilColor = map.getCeilColor();

    gameManager = make_shared<GameManager>(&fpsController);
    gameManager->init();
    guiManager = make_shared<GuiManager>(gameManager);
}

// these two functions are very similar, but I'd rather keep them separate because one might want to add some weird stuff to either floor or ceiling and the "duplication" actually splits the responsibilities better
void Wolf3D::renderFloor(RenderPass &renderPass)
{
    static auto floor = Mesh::create().withQuad(100.f).build();
    static shared_ptr<Material> material = Shader::getUnlit()->createMaterial();
    material->setColor(sre::Color(floorColor));
    vec3 position = vec3(0, -0.5, 0);
    mat4 rotateTrans = mat4(1.f);
    rotateTrans = glm::rotate(rotateTrans, glm::radians(90.0f), glm::vec3(-1.0f, 0.0f, 0.0f));
    renderPass.draw(floor, glm::translate(position) * rotateTrans, material);
}

void Wolf3D::renderCeiling(RenderPass &renderPass)
{
    static auto ceil = Mesh::create().withQuad(100.f).build();
    static shared_ptr<Material> material = Shader::getUnlit()->createMaterial();
    material->setColor(sre::Color(ceilColor));
    vec3 position = vec3(0, 0.5, 0);
    mat4 rotateTrans = mat4(1.f);
    rotateTrans = glm::rotate(rotateTrans, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    renderPass.draw(ceil, glm::translate(position) * rotateTrans, material);
}

void Wolf3D::renderDebugBricks(RenderPass &renderPass)
{
    static auto cube = Mesh::create().withCube(0.5f).build();
    static vector<shared_ptr<Material>> materials = {
        Shader::getUnlit()->createMaterial(),
        Shader::getUnlit()->createMaterial(),
        Shader::getUnlit()->createMaterial()};

    std::vector<vec3> positions = {
        {-1, 0, -2},
        {0, 0, -3},
        {1, 0, -4}};
    std::vector<sre::Color> colors = {
        {1, 0, 0, 1},
        {0, 1, 0, 1},
        {0, 0, 1, 1},
    };
    for (int i = 0; i < positions.size(); i++)
    {
        materials[i]->setColor(colors[i]);
        renderPass.draw(cube, glm::translate(positions[i]), materials[i]);
    }
}

// int main()
// {
//     _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
//     new Wolf3D();
//     _CrtDumpMemoryLeaks();
//     return 0;
// }