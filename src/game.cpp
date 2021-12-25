#include <game.h>

using namespace glm;
using namespace std;

void Game::init()
{
    glEnable(GL_DEPTH_TEST);
    scrWidth = 1920, scrHeight = 1080;
    currentTime = deltaTime = 0.0f;
    cameraState = 0;
    initSky();
}

void Game::logic()
{
}

void Game::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    mat4 view = cam[cameraState].getViewMatrix();
    mat4 projection = perspective(radians(cam[0].zoom), scrWidth / scrHeight, 0.1f, 100.0f);



    updateSky(projection, view);
    drawSky();
}

void Game::initSky()
{
    skyShader = getShaderProgram("../glsl/skybox.vs", "../glsl/skybox.fs");
    sky.init();
}

void Game::updateSky(const glm::mat4 &projection, const glm::mat4 &view)
{
    mat4 view3 = mat4(mat3(view));
    skyShader.use();
    skyShader.setMat4("projection", projection);
    skyShader.setMat4("view", view3);
    glUseProgram(0);
}

void Game::drawSky()
{
    sky.draw(skyShader);
}