#include "ResourceLoader.h"
#include <iostream>
#include <fstream>
#include <sstream>

std::unordered_map<std::string, unsigned> ResourceLoader::shaders = { };

std::string ResourceLoader::createShader(const std::string& filePath)
{
    const std::string extensions[] = { "/vertex.glsl", "/fragment.glsl" };
    std::string codes[] = { "", "" };
    std::ifstream streams[] = { { }, { } };
    const GLenum types[] = { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER };
    unsigned shaders[] = { 0, 0 };


    int success;
    char infoLog[512];
    for (short i = 0; i < 2; i++) {
        const std::string& extension = extensions[i];
        std::string& code = codes[i];
        std::ifstream& stream = streams[i];
        const GLenum& type = types[i];
        unsigned& shader = shaders[i];

        stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            // open files
            stream.open(filePath + extension);
            std::stringstream s;
            // read file's buffer contents into streams
            s << stream.rdbuf();
            // close file handlers
            stream.close();
            // convert stream into string
            code = s.str();
        }
        catch (std::ifstream::failure e)
        {
            std::cout << filePath + extension << " not read" << std::endl;
        }
        const char* c = code.c_str();

        shader = glCreateShader(type);
        glShaderSource(shader, 1, &c, NULL);
        glCompileShader(shader);

        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            std::cout << filePath + extension << " faild to compile" << infoLog << std::endl;
        }
    }

    // link shaders
    int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, shaders[0]); // vertex
    glAttachShader(shaderProgram, shaders[1]); // fragment
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << filePath << " faild to link " << infoLog << std::endl;
    }
    glDeleteShader(shaders[0]);
    glDeleteShader(shaders[1]);

    auto shader_s = Utils::split(filePath, " ");
    std::string name = shader_s.back();
    
    ResourceLoader::shaders[name] = shaderProgram;

    return name;
}

const unsigned ResourceLoader::getShader(const std::string& name)
{
    auto& shaders = ResourceLoader::shaders;
    unsigned s = shaders.size();
    unsigned r = shaders[name];
    if (s < shaders.size()) {
        r = (*shaders.begin()).second;
        shaders.erase(name);
    }
    return r;
}

void ResourceLoader::cleanUp()
{
    for (const auto& item : shaders) {
        glDeleteShader(item.second);
    }
}
