#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>

class Shader
{
public:
    unsigned int ID;
    unsigned int type;
    Shader(const std::string &filePath, unsigned int type);
private:
};

class ShaderProgram
{

};