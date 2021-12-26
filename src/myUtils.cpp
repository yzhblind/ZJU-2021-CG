#include <myUtils.h>

using namespace glm;
using namespace std;

inline mat3 calcNormal(const mat4 &t) { return mat3(transpose(inverse(t))); }

void MatrixStack::reset()
{
    modelStack.clear(), normalStack.clear();
    modelStack.push_back(mat4(1.0f));
    normalStack.push_back(mat3(1.0f));
}
void MatrixStack::rotate(float angle, vec3 axis)
{
    mat4 &t = modelStack.back();
    mat4 T(1.0f);
    T = glm::rotate(T, radians(angle), axis);
    t = T * t;
    normalStack.back() = calcNormal(t);
}
void MatrixStack::scale(vec3 factor)
{
    mat4 &t = modelStack.back();
    mat4 T(1.0f);
    T = glm::scale(T, factor);
    t = T * t;
    normalStack.back() = calcNormal(t);
}
void MatrixStack::translate(glm::vec3 direction)
{
    mat4 &t = modelStack.back();
    mat4 T(1.0f);
    T = glm::translate(T, direction);
    t = T * t;
    normalStack.back() = calcNormal(t);
}
void MatrixStack::mul(const mat4 &trans)
{
    mat4 &t = modelStack.back();
    t = trans * t;
    normalStack.back() = calcNormal(t);
}