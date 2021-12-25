#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <shader.h>
#include <texture.h>

class Skybox
{
public:
    void init();
    void draw(ShaderProgram &skyShader);

private:
    unsigned int VAO, VBO;
    unsigned int whichSky;
    unsigned int skyTexture[2];

    unsigned int loadCubeMap(const std::vector<std::string> &faces);
};