#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/scene.h>

#include <texture.h>
#include <mesh.h>

#include <map>
#include <string>
#include <vector>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

class Model
{
public:
    std::map<std::string, Texture> texTable;
    std::vector<Mesh> meshes;
    std::string dir;

    Model(const std::string &path) { loadModel(path); }

    void Draw(ShaderProgram &prgm)
    {
        for (unsigned int i = 0; i < meshes.size(); ++i)
            meshes[i].draw(prgm);
    }

private:
    void loadModel(string const &path);
    void loadMeshes(const aiScene *scene);
    void loadMesh(aiMesh *mesh, Mesh &myMesh, aiMaterial *material);
    void loadTexFromMat(aiMaterial *mat, Mesh &myMesh, aiTextureType type, TextureType myType);
};