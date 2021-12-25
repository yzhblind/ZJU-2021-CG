#include "camera.h"

#include <cmath>

using namespace glm;
using namespace std;

const vec3 initUp = vec3(0.0f, 1.0f, 0.0f);
const float initSpeed = 5.0f;
const float initSensitivity = 0.05f;
const float initZoom = 45.0f;

inline void clamp(float &x, float a, float b)
{
    x = (x < a) ? a : x;
    x = (x > b) ? b : x;
}

Camera::Camera(vec3 pos, float yaw, float pitch) : pos(pos), yaw(yaw), pitch(pitch)
{
    up = initUp;
    speed = initSpeed;
    sensitivity = initSensitivity;
    zoom = initZoom;
    setFR();
}

void Camera::move(CameraDir dir, float t)
{
    float dis = t * speed;
    switch (dir)
    {
    case FORWARD:
        pos += front * dis;
        break;
    case BACKWARD:
        pos -= front * dis;
        break;
    case LEFT:
        pos -= right * dis;
        break;
    case RIGHT:
        pos += right * dis;
        break;
    case LEVEL_FORWARD:
        pos += normalize(vec3(front.x, 0.0f, front.z)) * dis;
        break;
    case LEVEL_BACKWARD:
        pos -= normalize(vec3(front.x, 0.0f, front.z)) * dis;
        break;
    case LEVEL_LEFT:
        pos -= normalize(vec3(right.x, 0.0f, right.z)) * dis;
        break;
    case LEVEL_RIGHT:
        pos += normalize(vec3(right.x, 0.0f, right.z)) * dis;
        break;
    }
}

void Camera::rotate(float tx, float ty)
{
    float disx = sensitivity * tx;
    float disy = sensitivity * ty;
    yaw += disx;
    pitch += disy;
    clamp(pitch, -89.0f, 89.0f);
    setFR();
}

void Camera::zoomInOut(float t)
{
    zoom -= t;
    clamp(zoom, 1.0f, 60.0f);
}

void Camera::setFR()
{
    front.y = sin(radians(pitch));
    front.x = cos(radians(pitch)) * cos(radians(yaw));
    front.z = cos(radians(pitch)) * sin(radians(yaw));
    front = normalize(front);
    right = normalize(cross(front, up));
}