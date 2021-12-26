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
    light0.direction = vec3(1.0f, -1.0f, 1.0f);
    light0.ambient = vec3(0.15f);
    light0.diffuse = vec3(1.0f);
    light0.specular = vec3(1.0f);
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

void Game::processLight(bool i, bool j, bool k, bool l)
{
    if (i && !k)
    {
        light0.direction.x += 0.1;
    }
    else if (!i && k)
    {
        light0.direction.x -= 0.1;
    }
    if (j && !l)
    {
        light0.direction.z += 0.1;
    }
    else if (!j && l)
    {
        light0.direction.z -= 0.1;
    }

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
    useLight(normalShader, light0);
    normalShader.setBool("en_light1", false);
    normalShader.setVec3("viewPos", cam[cameraState].pos);
    //Frustum c(6,3,4,6,glm::vec3(10,10,10));
    //c.draw(normalShader);

    turret->draw(normalShader);
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