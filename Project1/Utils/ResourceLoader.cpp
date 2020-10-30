#include "ResourceLoader.h"
#include <array>
#include <iostream>
#include <fstream>
#include <sstream>
#define STB_IMAGE_IMPLEMENTATION
#include "../stb_image.h"

std::string ResourceLoader::defaultShaderName = "";
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

    glDetachShader(shaderProgram, shaders[0]);
    glDetachShader(shaderProgram, shaders[1]);
    glDeleteShader(shaders[0]);
    glDeleteShader(shaders[1]);
    glUseProgram(0);

    auto shader_s = Utils::split(filePath, "/");
    std::string name = shader_s.back();
    
    ResourceLoader::shaders[name] = shaderProgram;

    if (ResourceLoader::defaultShaderName == "") {
        ResourceLoader::defaultShaderName = name;
    }

    return name;
}

const unsigned ResourceLoader::getShader(const std::string& name)
{
    auto& shaders = ResourceLoader::shaders;
    unsigned s = shaders.size();
    unsigned r = shaders[name];
    if (s < shaders.size()) {
        r = ResourceLoader::getShader(ResourceLoader::defaultShaderName);
        shaders.erase(name);
    }
    return r;
}

const unsigned ResourceLoader::createTexture(const std::string& filePath, const TextureType type, const bool& flip)
{
    stbi_set_flip_vertically_on_load(flip);
    unsigned tex = 0;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    int width, height, nrChannels;
    unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);
    GLenum t = GL_SRGB;
    switch (nrChannels)
    {
    case 1:
        t = GL_RED;
        break;
    case 3:
        t = GL_RGB;
        break;
    case 4:
        t = GL_RGBA;
        break;
    };
    // unsigned char data[] = { 255, 127, 127 };
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, t, width, height, 0, t, GL_UNSIGNED_BYTE, data);
        // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture: " << filePath << std::endl;
    }
    // stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);

    return tex;
}

void ResourceLoader::cleanUp()
{
    for (const auto& item : shaders) {
        glDeleteShader(item.second);
    }
}

void ResourceLoader::createModel(const std::string& filePath)
{
}
