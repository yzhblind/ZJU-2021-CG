#include <game.h>
#include<basic.h>
using namespace glm;
using namespace std;

void Game::init()
{
    glEnable(GL_DEPTH_TEST);
    scrWidth = 1920, scrHeight = 1080;
    currentTime = deltaTime = 0.0f;
    cameraState = 0;
    initTurret();
    initSky();
}

void Game::logic()
{
}

void Game::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    mat4 view = cam[cameraState].getViewMatrix();
    mat4 projection = perspective(radians(cam[cameraState].zoom), scrWidth / scrHeight, 0.1f, 100.0f);
    // modelStack.push();
    // modelStack.scale(vec3(0.01f));
    drawTurret(projection, view);
    // modelStack.pop();

    updateSky(projection, view);
    drawSky();
}

void Game::processKeyMove(bool w, bool a, bool s, bool d)
{
    if (w && !s)
        cam[cameraState].move(cameraState == 0 ? FORWARD : LEVEL_FORWARD, deltaTime);
    else if (!w && s)
        cam[cameraState].move(cameraState == 0 ? BACKWARD : LEVEL_BACKWARD, deltaTime);

    if (a && !d)
        cam[cameraState].move(cameraState == 0 ? LEFT : LEVEL_LEFT, deltaTime);
    else if (!a && d)
        cam[cameraState].move(cameraState == 0 ? RIGHT : LEVEL_RIGHT, deltaTime);
}

void Game::processMouseMove(double xoffset, double yoffset)
{
    cam[cameraState].rotate(xoffset, yoffset);

}

void Game::initTurret()
{
    normalShader = getShaderProgram("../glsl/normal.vs", "../glsl/normal.fs");
    turret = new Model("../resources/models/turret1/turret.obj");
    // turret = new Model("../resources/models/test/nanosuit.obj");
}

void Game::drawTurret(const glm::mat4 &projection, const glm::mat4 &view)
{
    normalShader.use();
    normalShader.setMat4("projection", projection);
    normalShader.setMat4("view", view);
    normalShader.setMat4("model", modelStack.getModelMatrix());
    normalShader.setMat3("normalMatrix", modelStack.getNormalMatrix());
    normalShader.setVec3("lightPos", vec3(1.0f, 0.5f, 2.0f));
    normalShader.setVec3("viewPos", cam[cameraState].pos);
    Frustum c(6,3,4,6);
    c.draw(normalShader);

    //turret->draw(normalShader);
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