#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>

#include <stb_image.h>

#include <map>
#include <string>
#include <vector>
#include <iostream>

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
    std::string name;
    Texture() {}
    Texture(unsigned int id, TextureType type, const std::string &name) : id(id), type(type), name(name) {}
};

unsigned int loadTexFromFile(const std::string &name, const std::string &dir);