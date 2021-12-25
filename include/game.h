#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <myUtils.h>
#include <camera.h>
#include <skybox.h>

class Game
{
public:
    void init();
    void logic();
    void render();
    void setTime(float t)
    {
        deltaTime = t - currentTime;
        currentTime = t;
    }

private:
    // 判定自由视角与塔防视角切换
    float scrWidth, scrHeight;
    int cameraState;
    Camera cam[2];
    float currentTime;
    float deltaTime;

    Skybox sky;
    ShaderProgram skyShader;

    void initSky();
    void updateSky(const glm::mat4 &projection, const glm::mat4 &view);
    void drawSky();
};