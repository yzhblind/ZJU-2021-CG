#include "shader.h"

#include <fstream>
#include <iostream>
#include <streambuf>

Shader::Shader(const std::string &filePath, unsigned int type) : type(type)
{
    std::ifstream shaderFile(filePath);
    std::string code{std::istreambuf_iterator<char>{shaderFile}, {}};
    shaderFile.close();
    const char *codeStr = code.c_str();
    ID = glCreateShader(type);
    glShaderSource(ID, 1, &codeStr, NULL);
    glCompileShader(ID);
    
}