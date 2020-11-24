#include "ResourceLoader.h"
#include <array>
#include <iostream>
#include <fstream>
#include <sstream>
#include <Images/STBImage.h>
#include "AssimpWrapper.h"


std::string ResourceLoader::defaultShaderName = "";
std::unordered_map<std::string, unsigned> ResourceLoader::shaders = { };
std::unordered_map<std::string, unsigned> ResourceLoader::textures = { };
std::list<Primative::VertexBuffer> ResourceLoader::buffers = { };
std::unordered_map<std::string, std::vector<unsigned>> ResourceLoader::models = { };

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

    // auto shader_s = Utils::split(filePath, "/");
    const std::string name = Utils::getFileName(filePath);
    
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
    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);
    textures.emplace(Utils::getFileName(filePath, 1), tex);
    return tex;
}

const unsigned ResourceLoader::getTexture(const std::string& name)
{
    auto& texs = ResourceLoader::textures;
    unsigned s = texs.size();
    unsigned r = texs[name];
    if (s < texs.size()) {
        r = 1;
        texs.erase(name);
    }
    return r;
}

void ResourceLoader::cleanUp()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
    for (const auto& item : shaders) {
        glDeleteShader(item.second);
    }
    shaders.clear();
    for (const auto& item : textures) {
        glDeleteTextures(1, &item.second);
    }
    textures.clear();
    for (auto& item : buffers) {
        item.cleanUp();
    }
    buffers.clear();
    models.clear();
}

const std::vector<Primative::VertexBuffer*> ResourceLoader::processMeshData(std::vector<Primative::Mesh*>& data, const std::string& name, GLenum draw_type)
{
    std::vector<Primative::VertexBuffer*> res;
    if (data.empty()) {
        return res;
    }
    auto& model = models[name];
    for (auto& m : data) {
        buffers.emplace_back(*m, draw_type);
        res.push_back(&buffers.back());
        model.push_back(buffers.size() - 1);
        delete m;
        m = nullptr;
    }
    data.clear();
    return res;
}

const std::vector<Primative::VertexBuffer*> ResourceLoader::createModel(const std::string& filePath, GLenum draw_type)
{
    std::vector<Primative::VertexBuffer*> res;
    std::vector<Primative::Mesh*> data = FileReaders::AssimpWrapper::loadModel(filePath);
    const std::string name = Utils::getFileName(filePath, 1);
    return processMeshData(data, name, draw_type);
}

const std::vector<Primative::VertexBuffer*> ResourceLoader::createModel(Primative::Mesh* meshes, const std::string& name, GLenum draw_type)
{
    std::vector<Primative::Mesh*> data = { meshes };
    return createModel(data, name, draw_type);
}

const std::vector<Primative::VertexBuffer*> ResourceLoader::createModel(std::vector<Primative::Mesh*>& meshes, const std::string& name, GLenum draw_type) {
    return processMeshData(meshes, name, draw_type);
}

const std::vector<Primative::VertexBuffer*> ResourceLoader::getModel(const std::string& name)
{
    unsigned s = models.size();
    auto& r = models[name];
    if (s < models.size()) {
        models.erase(name);
    }
    std::vector<Primative::VertexBuffer*> res;
    for (unsigned& i : r) {
        res.push_back(&Utils::elementAt(buffers, i));
    }
    return res;
}
