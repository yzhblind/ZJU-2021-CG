#include <mesh.h>

#include <iostream>

using namespace glm;
using namespace std;

void Mesh::createObject()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
}
void Mesh::sendData()
{
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
}
void Mesh::setAttribute()
{
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, texCoords));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, tan));
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, biTan));
}

void Mesh::init()
{
    cout << "vertex array size: " << vertices.size() << endl;
    cout << "index array size: " << indices.size() << endl;
    cout << "texture array size: " << textures.size() << endl;
    createObject();
    sendData();
    setAttribute();
    recover();
}

void Mesh::setTexture(ShaderProgram &prgm)
{
    unsigned int diffuseCnt = 0;
    unsigned int specularCnt = 0;
    unsigned int normalCnt = 0;
    unsigned int heightCnt = 0;
    string name;
    for (unsigned int i = 0; i < textures.size(); ++i)
    {
        glActiveTexture(GL_TEXTURE0 + i + 1);
        switch (textures[i].type)
        {
        case DIFFUSE_T:
            // name = "diffuse_texture" + to_string(diffuseCnt++);
            name = "diffuse_texture";
            diffuseCnt++;
            break;
        case SPECULAR_T:
            // name = "specular_texture" + to_string(specularCnt++);
            name = "specular_texture";
            specularCnt++;
            break;
        case NORMAL_T:
            // name = "normal_texture" + to_string(normalCnt++);
            name = "normal_texture";
            normalCnt++;
            break;
        // case HEIGHT_T:
        //     // name = "height_texture" + to_string(heightCnt++);
        //     name = "height_texture";
        //     heightCnt++;
        //     break;
        }
        prgm.setInt(name, i + 1);
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }
    prgm.setBool("en_diffuse", diffuseCnt > 0);
    prgm.setBool("en_specular", specularCnt > 0);
    prgm.setBool("en_normal", normalCnt > 0);
}
void Mesh::drawCall()
{
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}
void Mesh::recover()
{
    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
}

void Mesh::draw(ShaderProgram &prgm)
{
    setTexture(prgm);
    drawCall();
    recover();
}