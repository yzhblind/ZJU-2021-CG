#include <game.h>
#include <basic.h>

#include <random>
#include <algorithm>

using namespace glm;
using namespace std;

void Game::init()
{
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    scrWidth = 1920, scrHeight = 1080;
    currentTime = deltaTime = 0.0f;
    cameraState = 0;
    initLight();
    initShadow();
    initModel();
    initLine();
    initSky();
}

void Game::initLine()
{
    lineShader = getShaderProgram("../glsl/line.vs", "../glsl/line.fs");
    glGenVertexArrays(1, &lineVAO);
    glGenBuffers(1, &lineVBO);
}

void Game::initShadow()
{
    shadowShader = getShaderProgram("../glsl/shadow.vs", "../glsl/shadow.fs");
    shadowWidth = 8192, shadowHeight = 4096;
    glGenFramebuffers(1, &shadowFBO);
    glGenTextures(1, &shadowMap);

    glBindTexture(GL_TEXTURE_2D, shadowMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowWidth, shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = {1.0, 1.0, 1.0, 1.0};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Game::initLight()
{
    en_light1 = false;
    light0.direction = vec3(1.0f, -1.0f, 1.0f);
    light0.ambient = vec3(0.15f);
    light0.diffuse = vec3(1.0f);
    light0.specular = vec3(1.0f);
    light1.ambient = vec3(0.0f, 0.0f, 0.2f);
    light1.diffuse = vec3(0.0f, 0.0f, 1.0f);
    light1.specular = vec3(0.0f, 0.0f, 1.0f);
    light1.constant = 1.0f;
    light1.linear = 0.7f;
    light1.quadratic = 1.8f;
}

void Game::logic()
{
}
float baseAngle = 0.0f;
void Game::drawScene(const glm::mat4 &projection, const glm::mat4 &view, ShaderProgram &prgm)
{
    modelStack.push();
    modelStack.translate(vec3(-0.3f, -0.32f, 0.0f));
    baseAngle += deltaTime * 5;
    modelStack.rotate(baseAngle, vec3(0.0f, 1.0f, 0.0f));
    modelStack.push();
    for (int i = 0; i < 10; i += 2)
    {
        for (int j = 0; j < 20; j += 2)
        {
            modelStack.push();
            modelStack.translate(vec3((float)i * 4, 0.0f, (float)j * 4));
            drawModel(projection, view, turret, prgm);
            modelStack.pop();
        }
    }
    modelStack.pop();
    modelStack.pop();
    for (int i = 10; i < 20; i += 2)
    {
        for (int j = 0; j < 20; j += 2)
        {
            modelStack.push();
            modelStack.translate(vec3((float)i * 4, 0.0f, (float)j * 4));
            drawModel(projection, view, virus, prgm);
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
            if (((i + j) & 1) == 0)
                drawModel(projection, view, box, prgm);
            else
                drawModel(projection, view, box2, prgm);
            modelStack.pop();
        }
    }
    modelStack.push();
    modelStack.translate(vec3(12.0f, 4.0f, 12.0f));
    drawModel(projection, view, box, prgm);
    modelStack.pop();
    modelStack.pop();

    modelStack.push();
    modelStack.scale(vec3(0.5f));
    // modelStack.mul(rotateMat);
    modelStack.translate(vec3(20.0f, 0.0f, 20.0f));
    en_light1 = true;
    light1.position = vec3(20.0f, 0.0f, 20.0f);
    drawModel(projection, view, sphere, prgm);
    modelStack.pop();
}
// projection & view is not in use here
void Game::shadowGen(const glm::mat4 &projection, const glm::mat4 &view)
{
    Camera &c = cam[cameraState];
    vec3 viewCenter = vec3(40.0f, 0.0f, 40.0f);
    float orthoHeight = normalize(light0.direction).y < -0.7 ? 60.0f : 35.0f;
    float orthoWidth = 60.0f;
    if (c.front.y < -0.7 && c.pos.y < 15)
    {
        viewCenter = vec3(c.pos.x, 0.0f, c.pos.z);
        if (c.front.y < -0.8 && c.pos.y < 10)
        {
            orthoHeight = 10.0f;
            orthoWidth = 20.0f;
        }
        else
        {   orthoWidth = glm::clamp(20.0f + 400.0f * (c.front.y + 0.8f) + 8.0f * (c.pos.y - 10.0f), 20.0f, 60.0f);
            orthoHeight = glm::clamp(10.0f + (orthoHeight - 10.0f) * 10.0f * (c.front.y + 0.8f) + (orthoHeight - 10.0f) / 5.0f * (c.pos.y - 10.0f), 10.0f ,orthoHeight);
        }
    }

    mat4 lightProjection = ortho(-orthoWidth, orthoWidth, -orthoHeight, orthoHeight, 0.1f, 128.0f);
    vec3 lightPos = viewCenter - 50.0f * normalize(light0.direction);
    mat4 lightView = lookAt(lightPos, viewCenter, vec3(0.0f, 1.0f, 0.0f));
    mat4 lightSpaceMatrix = lightProjection * lightView;
    shadowShader.use();
    shadowShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
    glViewport(0, 0, shadowWidth, shadowHeight);
    glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    glCullFace(GL_FRONT);
    drawScene(projection, view, shadowShader);
    glCullFace(GL_BACK);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, scrWidth, scrHeight);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    normalShader.use();
    normalShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, shadowMap);
    normalShader.setInt("shadowMap", 0);
}

float rand_f() { return (((float)rand() / (float)RAND_MAX) - 0.5); }

void Game::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    mat4 view = cam[cameraState].getViewMatrix();
    mat4 projection = perspective(radians(cam[cameraState].zoom), scrWidth / scrHeight, 0.1f, 200.0f);

    // rotateMat = rotate(rotateMat, radians(deltaTime * 10), vec3(rand_f(), 0.0f, rand_f()));

    shadowGen(projection, view);

    drawScene(projection, view, normalShader);

    vector<vec3> lines = {vec3(2.0f, 0.25f, 2.0f), vec3(2.0f, 0.25f, 60.0f)};
    drawLine(lines, projection, view, lineShader);

    updateSky(projection, view);
    drawSky();
}

void Game::drawLine(const std::vector<glm::vec3> &lines, const glm::mat4 &projection, const glm::mat4 &view, ShaderProgram &prgm)
{
    prgm.use();
    prgm.setMat4("projection", projection);
    prgm.setMat4("view", view);
    glBindVertexArray(lineVAO);
    glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
    glBufferData(GL_ARRAY_BUFFER, lines.size() * sizeof(vec3), &lines[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (void *)0);
    glDrawArrays(GL_LINES, 0, 2);
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

void Game::initModel()
{
    normalShader = getShaderProgram("../glsl/normal.vs", "../glsl/normal.fs");
    turret = new Model("../resources/models/turret1/turret.obj");
    box = new Model("../resources/models/woodbox/Wooden Crate.obj");
    virus = new Model("../resources/models/virusLow/virus.obj");
    box2 = new Model("../resources/models/woodbox2/Cube_obj.obj");
    vector<Mesh *> t;
    t.push_back(new Sphere(1.0f));
    sphere = new Model(t);
    // rotateMat = mat4(1.0f);
    // turret = new Model("../resources/models/test/nanosuit.obj");
}

void Game::drawModel(const glm::mat4 &projection, const glm::mat4 &view, Model *m, ShaderProgram &prgm)
{
    prgm.use();
    prgm.setMat4("projection", projection);
    prgm.setMat4("view", view);
    prgm.setMat4("model", modelStack.getModelMatrix());
    prgm.setMat3("normalMatrix", modelStack.getNormalMatrix());
    useLight(prgm, light0);
    if (en_light1 == false)
        prgm.setBool("en_light1", false);
    else
        useLight(prgm, light1);
    prgm.setVec3("viewPos", cam[cameraState].pos);
    m->draw(prgm);
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