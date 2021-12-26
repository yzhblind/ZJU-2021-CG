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

class Game
{
public:
    void init();
    void logic();
    void render();
    void processKeyMove(bool w, bool a, bool s, bool d);
    void processMouseMove(double xoffset, double yoffset);
    void processLight(bool i, bool j, bool k, bool l);
    void switchCamera() { cameraState ^= 1; };
    void zoomInOutCamera(double yoffset)
    {
        cam[cameraState].zoomInOut(yoffset);
    }
    void switchState(){ state ^= 1; }
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

    dirLight light0;
    pointLight light1;

    Model *turret;
    ShaderProgram normalShader;

    void initTurret();
    void drawTurret(const glm::mat4 &projection, const glm::mat4 &view);

    Skybox sky;
    ShaderProgram skyShader;

    void initSky();
    void updateSky(const glm::mat4 &projection, const glm::mat4 &view);
    void drawSky();

    bool state;
};