#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <shader.h>

struct dirLight
{
    glm::vec3 direction;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

struct pointLight
{
    glm::vec3 position;

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct spotLight
{
    glm::vec3 position;
    glm::vec3 direction;
    float cutOff;

    float constant;
    float linear;
    float quadratic;
};

void useLight(ShaderProgram &prgm, const dirLight &dir);
void useLight(ShaderProgram &prgm, const pointLight &point);
void useLight(ShaderProgram &prgm, const spotLight &spot);