#pragma once

#include <glad/glad.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class MatrixStack
{
public:
private:
};

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
    float currentTime;
    float deltaTime;
};