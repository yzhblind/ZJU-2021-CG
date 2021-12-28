#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum CameraDir
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    LEVEL_FORWARD,
    LEVEL_BACKWARD,
    LEVEL_LEFT,
    LEVEL_RIGHT
};

class Camera
{
public:
    glm::vec3 pos;
    glm::vec3 up;
    glm::vec3 front;
    glm::vec3 right;
    float yaw;
    float pitch;
    float speed;
    float sensitivity;
    float zoom;

    Camera(glm::vec3 position = glm::vec3(0.0f, 40.0f, 5.0f), float yaw = -90.0f, float pitch = -60.0f);
    void move(CameraDir dir, float t);
    void rotate(float tx, float ty);
    void zoomInOut(float t);
    void zoom2fit() { zoom = 45.0f; }
    glm::mat4 getViewMatrix() { return glm::lookAt(pos, pos + front, up); }

private:
    void setFR();
};