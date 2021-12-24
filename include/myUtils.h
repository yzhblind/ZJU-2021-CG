#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

class MatrixStack
{
public:
    MatrixStack() { reset(); }
    void reset();
    void push() { modelStack.push_back(modelStack.back()), normalStack.push_back(normalStack.back()); }
    void pop() { modelStack.pop_back(), normalStack.pop_back(); }
    glm::mat4 getModelMatrix() { return modelStack.back(); }
    glm::mat3 getNormalMatrix() { return normalStack.back(); }
    void rotate(float angle, glm::vec3 axis);
    void scale(glm::vec3 factor);
    void translate(glm::vec3 direction);
    void mul(const glm::mat4 &trans);

private:
    std::vector<glm::mat4> modelStack;
    std::vector<glm::mat3> normalStack;
};