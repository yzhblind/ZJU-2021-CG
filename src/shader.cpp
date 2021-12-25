#include <shader.h>

#include <fstream>
#include <iostream>
#include <streambuf>

#include <exception>

std::string type2str(GLuint ID)
{
    switch (ID)
    {
    case GL_VERTEX_SHADER:
        return "Vertex Shader";
        break;
    case GL_FRAGMENT_SHADER:
        return "Fragment Shader";
        break;
    case GL_GEOMETRY_SHADER:
        return "Geometry Shader";
        break;
    default:
        return "Unknown Shader";
        break;
    }
}

Shader::Shader(const std::string &filePath, GLenum type) : type(type)
{
    std::ifstream shaderFile(filePath);
    std::string code{std::istreambuf_iterator<char>{shaderFile}, {}};
    shaderFile.close();
    const char *codeStr = code.c_str();
    ID = glCreateShader(type);
    glShaderSource(ID, 1, &codeStr, NULL);
    glCompileShader(ID);
}

void Shader::checkError()
{
    GLint success;
    GLchar *infoLog;
    glGetShaderiv(ID, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        infoLog = new GLchar[4096];
        glGetShaderInfoLog(ID, 4096, NULL, infoLog);
        std::cerr << "Error of " << type2str(ID) << std::endl;
        std::cerr << infoLog << std::endl;
        throw std::exception();
    }
}

ShaderProgram::ShaderProgram(const Shader &vertexShader, const Shader &fragmentShader)
{
    ID = glCreateProgram();
    glAttachShader(ID, vertexShader.getID());
    glAttachShader(ID, fragmentShader.getID());
    glLinkProgram(ID);
}

ShaderProgram::ShaderProgram(const Shader &vertexShader, const Shader &fragmentShader, const Shader &geometryShader)
{
    ID = glCreateProgram();
    glAttachShader(ID, vertexShader.getID());
    glAttachShader(ID, geometryShader.getID());
    glAttachShader(ID, fragmentShader.getID());
    glLinkProgram(ID);
}

void ShaderProgram::checkError()
{
    GLint success;
    GLchar *infoLog;
    glGetShaderiv(ID, GL_LINK_STATUS, &success);
    if (!success)
    {
        infoLog = new GLchar[4096];
        glGetProgramInfoLog(ID, 4096, NULL, infoLog);
        std::cerr << "Error of Program" << std::endl;
        std::cerr << infoLog << std::endl;
        throw std::exception();
    }
}

ShaderProgram getShaderProgram(const std::string &vertexPath, const std::string &fragmentPath)
{
    Shader vertexShader(vertexPath, GL_VERTEX_SHADER);
    Shader fragmentShader(fragmentPath, GL_FRAGMENT_SHADER);
    ShaderProgram prgm(vertexShader, fragmentShader);
    vertexShader.clear(), fragmentShader.clear();
    return prgm;
}
ShaderProgram getShaderProgram(const std::string &vertexPath, const std::string &fragmentPath, const std::string &geometryPath)
{
    Shader vertexShader(vertexPath, GL_VERTEX_SHADER);
    Shader fragmentShader(fragmentPath, GL_FRAGMENT_SHADER);
    Shader geometryShader(geometryPath, GL_GEOMETRY_SHADER);
    ShaderProgram prgm(vertexShader, fragmentShader, geometryShader);
    vertexShader.clear(), fragmentShader.clear(), geometryShader.clear();
    return prgm;
}