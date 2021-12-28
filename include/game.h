#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <myUtils.h>
#include <camera.h>
#include <skybox.h>
#include <model.h>
#include <light.h>
#include <Glogic.h>

class Game
{
public:
    _MAP _M;
    void MAP_init();
    void upd(); //传出当前场景
    void setT(int x, int y); //放塔
    void setE(int x, int y, int cd=CD_default); //放出怪点
    void delT(int x, int y); //撤塔
    void delE(int x, int y); //撤出怪点
    void setHome(int x, int y); //设置家
    void setWall(int x, int y); //设置家
    UPD data2draw;

public:
    void init();
    void logic();
    void render();
    void edit();
    void processKeyMove(bool w, bool a, bool s, bool d);
    void processMouseMove(double xoffset, double yoffset);
    void processRotate(bool q, bool e);
    void processLight(bool i, bool j, bool k, bool l);
    void switchCamera() { cameraState ^= 1; };
    int getCameraState() { return cameraState; }
    void zoomInOutCamera(double yoffset)
    {
        cam[cameraState].zoomInOut(yoffset);
    }
    void switchState() { state ^= 1; }
    glm::vec3 getCameraFront()
    {
        return cam[cameraState].front;
    }
    glm::vec3 getCameraPos()
    {
        return cam[cameraState].pos;
    }
    void setScrSize(int width, int height)
    {
        scrWidth = width, scrHeight = height;
        glViewport(0, 0, width, height);
    }
    void setTime(float t)
    {
        deltaTime = t - currentTime;
        currentTime = t;
    }
    bool getState()
    {
        return state;
    }

private:
    MatrixStack modelStack;

    float currentTime;
    float deltaTime;

    float scrWidth, scrHeight;
    int cameraState; // 判定自由视角与塔防视角切换
    Camera cam[2];

    void initShadow();
    void shadowGen(const glm::mat4 &projection, const glm::mat4 &view);

    unsigned int shadowWidth, shadowHeight;
    unsigned int shadowFBO;
    unsigned int shadowMap;
    ShaderProgram shadowShader;

    void initLight();

    dirLight light0;
    pointLight light1;

    void drawScene(const glm::mat4 &projection, const glm::mat4 &view, ShaderProgram &prgm);

    Model *turret;
    Model *box;
    Model *virus;
    bool en_light1;
    // glm::mat4 rotateMat;
    Model *box2;
    Model *sphere;
    ShaderProgram normalShader;

    void initModel();
    void drawModel(const glm::mat4 &projection, const glm::mat4 &view, Model *m, ShaderProgram &prgm);

    unsigned int lineVAO, lineVBO;
    ShaderProgram lineShader;
    vector<glm::vec3> lines;

    void initLine();
    void drawLine(const glm::mat4 &projection, const glm::mat4 &view, ShaderProgram &prgm);

    Skybox sky;
    ShaderProgram skyShader;

    void initSky();
    void updateSky(const glm::mat4 &projection, const glm::mat4 &view);
    void drawSky();

    bool state;
};