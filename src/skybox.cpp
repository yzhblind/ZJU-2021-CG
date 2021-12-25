#include <skybox.h>

using namespace glm;
using namespace std;

const float skyboxVertices[] =
    {-1.0f, 1.0f, -1.0f,
     -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, 1.0f, -1.0f,
     -1.0f, 1.0f, -1.0f,

     -1.0f, -1.0f, 1.0f,
     -1.0f, -1.0f, -1.0f,
     -1.0f, 1.0f, -1.0f,
     -1.0f, 1.0f, -1.0f,
     -1.0f, 1.0f, 1.0f,
     -1.0f, -1.0f, 1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, 1.0f,
     1.0f, 1.0f, 1.0f,
     1.0f, 1.0f, 1.0f,
     1.0f, 1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

     -1.0f, -1.0f, 1.0f,
     -1.0f, 1.0f, 1.0f,
     1.0f, 1.0f, 1.0f,
     1.0f, 1.0f, 1.0f,
     1.0f, -1.0f, 1.0f,
     -1.0f, -1.0f, 1.0f,

     -1.0f, 1.0f, -1.0f,
     1.0f, 1.0f, -1.0f,
     1.0f, 1.0f, 1.0f,
     1.0f, 1.0f, 1.0f,
     -1.0f, 1.0f, 1.0f,
     -1.0f, 1.0f, -1.0f,

     -1.0f, -1.0f, -1.0f,
     -1.0f, -1.0f, 1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     -1.0f, -1.0f, 1.0f,
     1.0f, -1.0f, 1.0f};

const vector<std::string> faces{
    "../resources/textures/skybox1/right.jpg",
    "../resources/textures/skybox1/left.jpg",
    "../resources/textures/skybox1/top.jpg",
    "../resources/textures/skybox1/bottom.jpg",
    "../resources/textures/skybox1/front.jpg",
    "../resources/textures/skybox1/back.jpg",
    "../resources/textures/skybox2/right.jpg",
    "../resources/textures/skybox2/left.jpg",
    "../resources/textures/skybox2/top.jpg",
    "../resources/textures/skybox2/bottom.jpg",
    "../resources/textures/skybox2/front.jpg",
    "../resources/textures/skybox2/back.jpg"};

unsigned int Skybox::loadCubeMap(const vector<std::string> &faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = whichSky * 6; i < whichSky * 6 + 6; ++i)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + (i % 6), 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

void Skybox::init()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    whichSky = 0;
    skyTexture[0] = loadCubeMap(faces);
    whichSky = 1;
    skyTexture[1] = loadCubeMap(faces);

    whichSky = 0;
}

void Skybox::draw(ShaderProgram &skyShader)
{
    skyShader.use();
    glDepthFunc(GL_LEQUAL);
    glBindVertexArray(VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyTexture[whichSky]);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS);
    glUseProgram(0);
}