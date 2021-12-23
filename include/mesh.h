#pragma once

#include <glad/glad.h> 

#include <glm/glm.hpp>

#include <shader.h>
#include <texture.h>

#include <string>
#include <vector>

struct Vertex
{
    glm::vec3 pos;
    glm::vec3 normal;
    glm::vec2 texCoords;
    glm::vec3 tan;
    glm::vec3 biTan;
};

class Mesh
{
public:
    Mesh() : VAO(0), VBO(0), EBO(0) { vertices.clear(), indices.clear(), textures.clear(); }
    void init();
    void draw(ShaderProgram &prgm);
    void push_back(const Vertex &e) { vertices.push_back(e); }
    void push_back(const unsigned int &e) { indices.push_back(e); }
    void push_back(const Texture &e) { textures.push_back(e); }

private:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    unsigned int VAO, VBO, EBO;

    void createObject();
    void sendData();
    void setAttribute();

    void setTexture(ShaderProgram &prgm);
    void drawCall();
    void recover();
};