#include <game.h>

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
    initModel();
    initSky();
}

void Game::logic()
{
}

void Game::drawScene(const glm::mat4 &projection, const glm::mat4 &view)
{
    modelStack.push();
    // modelStack.translate(vec3(-0.3f, -0.1f, 0.0f));
    modelStack.push();
    for (int i = 0; i < 10; ++i)
    {
        for (int j = 0; j < 20; ++j)
        {
            modelStack.push();
            modelStack.translate(vec3((float)i * 4, 0.0f, (float)j * 4));
            drawModel(projection, view, turret);
            modelStack.pop();
        }
    }
    modelStack.pop();
    modelStack.pop();
    for (int i = 10; i < 20; ++i)
    {
        for (int j = 0; j < 20; ++j)
        {
            modelStack.push();
            modelStack.translate(vec3((float)i * 4, 0.0f, (float)j * 4));
            drawModel(projection, view, virus);
            modelStack.pop();
        }
    }
    modelStack.push();
    modelStack.scale(vec3(2.0f));
    modelStack.translate(vec3(0.0f, -3.0f, 0.0f));
    for (int i = 0; i < 20; ++i)
    {
        for (int j = 0; j < 20; ++j)
        {
            modelStack.push();
            modelStack.translate(vec3((float)i * 4, 0.0f, (float)j * 4));
            drawModel(projection, view, box);
            modelStack.pop();
        }
    }
    modelStack.pop();
}

void Game::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    mat4 view = cam[cameraState].getViewMatrix();
    mat4 projection = perspective(radians(cam[cameraState].zoom), scrWidth / scrHeight, 0.1f, 200.0f);

    drawScene(projection, view);

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

void Game::initModel()
{
    normalShader = getShaderProgram("../glsl/normal.vs", "../glsl/normal.fs");
    turret = new Model("../resources/models/turret1/turret.obj");
    box = new Model("../resources/models/woodbox/Wooden Crate.obj");
    virus = new Model("../resources/models/virusLow/virus.obj");
    // turret = new Model("../resources/models/test/nanosuit.obj");
}

void Game::drawModel(const glm::mat4 &projection, const glm::mat4 &view, Model *m)
{
    normalShader.use();
    normalShader.setMat4("projection", projection);
    normalShader.setMat4("view", view);
    normalShader.setMat4("model", modelStack.getModelMatrix());
    normalShader.setMat3("normalMatrix", modelStack.getNormalMatrix());
    useLight(normalShader, light0);
    normalShader.setBool("en_light1", false);
    normalShader.setVec3("viewPos", cam[cameraState].pos);
    m->draw(normalShader);
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