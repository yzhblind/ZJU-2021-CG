#pragma once

#include <glad/glad.h> 

#include <glm/glm.hpp>

#include <string>
#include <vector>

enum TextureType
{
    DIFFUSE_T,
    SPECULAR_T,
    NORMAL_T,
    HEIGHT_T
};

struct Texture
{
    unsigned int id;
    TextureType type;
    std::string path;
};

class TexMgr
{

};