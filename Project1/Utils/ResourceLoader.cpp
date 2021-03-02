#include "ResourceLoader.h"
#include <array>
#include <iostream>
#include <fstream>
#include <sstream>
#include <Images/STBImage.h>
#include "AssimpWrapper.h"
#include "../Primatives/Material.h"
#include "../Primatives/Buffers.h"
#include "../Primatives/Model.h"

std::string ResourceLoader::defaultShaderName = "";
std::unordered_map<std::string, unsigned> ResourceLoader::shaders = { };
std::unordered_map<std::string, unsigned> ResourceLoader::textures = { };
std::vector<Primative::VertexBuffer> ResourceLoader::buffers = { };
std::unordered_map<std::string, Render::Animation::Animation> ResourceLoader::animations = { };
std::vector<Materials::Material*> ResourceLoader::materials = { };
std::unordered_map<std::string, Primative::Model> ResourceLoader::models = { };

std::string ResourceLoader::createShader(const std::string& filePath, bool hasGeom)
{
    const std::string extensions[] = { "/vertex.glsl", "/geometry.glsl", "/fragment.glsl" };
    std::string codes[] = { "", "", "" };
    std::ifstream streams[] = { { }, { }, { } };
    const GLenum types[] = { GL_VERTEX_SHADER, GL_GEOMETRY_SHADER, GL_FRAGMENT_SHADER };
    unsigned shaders[] = { 0, 0, 0 };

    int success;
    char infoLog[512];
    for (short i = 0; i < 3; i++) {
        if (NOT hasGeom AND i == 1)
            continue;
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
            std::cout << filePath + extension << " faild to compile " << infoLog << std::endl;
        }
    }

    // link shaders
    int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, shaders[0]); // vertex
    if(hasGeom)
        glAttachShader(shaderProgram, shaders[1]); // geometry
    glAttachShader(shaderProgram, shaders[2]); // fragment
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << filePath << " faild to link " << infoLog << std::endl;
    }

    glDetachShader(shaderProgram, shaders[0]);
    if(hasGeom)
        glDetachShader(shaderProgram, shaders[1]);
    glDetachShader(shaderProgram, shaders[2]);
    glDeleteShader(shaders[0]);
    glDeleteShader(shaders[1]);
    glDeleteShader(shaders[2]);
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

const Materials::PBR ResourceLoader::createPBR(String dirPath, std::vector<TextureType> types, std::vector<bool> flip)
{
    const std::vector<unsigned> textures = loadTextureFile(dirPath, types, flip);
    materials.push_back(new Materials::PBR());
    materials.pop_back();
    Materials::PBR& res = *dynamic_cast<Materials::PBR*>(materials.back());
    unsigned i = 0;
    for (TextureType& type : types) {
        switch (type)
        {
        case TextureType::NormalMap:
            res.getNormal() = textures[i];
            break;
        case TextureType::AlbedoMap:
            res.getAlbedo() = textures[i];
            break;
        case TextureType::MetalicMap:
            res.getMetalic() = textures[i];
            break;
        case TextureType::AOMap:
            res.getAO() = textures[i];
            break;
        case TextureType::RoughnessMap:
            res.getRoughness() = textures[i];
            break;
        }
        i++;
    }
    return *dynamic_cast<Materials::PBR*>(materials.back());
}

const std::vector<unsigned> ResourceLoader::loadTextureFile(String dirPath, std::vector<TextureType> types, std::vector<bool> flip)
{
    const int s = Utils::countFiles(dirPath) - 1;
    types.reserve(s);
    flip.reserve(s);
    while (types.size() < s) {
        types.push_back(TextureType::DiffuseMap);
    }
    while (types.size() > s)
    {
        types.pop_back();
    }
    while (flip.size() < s) {
        flip.push_back(1);
    }
    while (flip.size() > s)
    {
        flip.pop_back();
    }
    std::vector<unsigned> res;
    res.reserve(s);

    auto dirIter = std::filesystem::directory_iterator(dirPath);
    int i = 0;
    for (auto& file : dirIter)
    {
        i++;
        if (i == 1)
            continue;
        if (types[i-2] == TextureType::CubeMap)
            continue;
        res.push_back(loadTexture(file.path().string(), types[i-2], flip[i-2]));
    }
    return res;
}

const unsigned ResourceLoader::loadTexture(const std::string& filePath, const TextureType type, const bool& flip)
{
    stbi_set_flip_vertically_on_load(flip);
    unsigned tex = 0;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    
    int width, height, nrChannels;
    Utils::Timer ti;
    ti.reName("STBI load");
    ti.start();
    unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);
    ti.log();
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

const unsigned ResourceLoader::loadCubeMap(const std::string& dirPath, const std::string& extension, const bool& flip)
{
    stbi_set_flip_vertically_on_load(flip);
    unsigned tex = 0;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_CUBE_MAP, tex);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    static const std::array<std::string, 6> files = {
        "/px", "/nx", "/py", "/ny", "/pz", "/nz"
    };
    int width, height, nrChannels;
    for (char i = 0; i < 6; i++) {
        std::string path = dirPath + files[i] + extension;
        unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
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
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, t, width, height, 0, t, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
        }
        else
        {
            std::cout << "Failed to load cubeMap: " << path << std::endl;
        }
        stbi_image_free(data);
    }
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    textures.emplace(Utils::getFileName(dirPath, 0) + ".cm", tex);
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
    for (auto& m : models) {
        m.second.cleanUp();
    }
    for (auto& m : materials) {
        m->cleanUp();
    }
    models.clear();
    for (auto itt = animations.begin(); itt != animations.end();) {
        (*itt).second.cleanUp();
        itt = animations.erase(itt);
    }
    animations.clear();
}

const Primative::Model ResourceLoader::processMeshData(std::vector<Primative::Mesh*>& data, const std::string& name, GLenum draw_type, const bool deleteAble)
{
    std::vector<unsigned> res;
    if (data.empty()) {
        return Primative::Model();
    }
    auto& model = models[name];
    for (auto& mesh : data) {
        buffers.emplace_back(Primative::VertexBuffer(*mesh, draw_type, 35044U, mesh->name));
        res.push_back(buffers.size() - 1);
        model.addBuffer(buffers.size() - 1);
        if(deleteAble)
            delete mesh;
        mesh = nullptr;
    }
    return model;
}

const Primative::Model ResourceLoader::createModel(const std::string& filePath, GLenum draw_type)
{
    auto data = FileReaders::AssimpWrapper::loadModel(filePath);
    
    const std::string name = Utils::getFileName(filePath, 1);
    for (auto itt = std::get<1>(data).begin(); itt != std::get<1>(data).end(); itt++) {
        animations.insert({ (*itt).getName(), (*itt) });
    }
    Primative::Model model = processMeshData(std::get<0>(data), name, draw_type, true);
    model.setSkeleton(std::get<2>(data));
    return model;
}

const Primative::Model ResourceLoader::createModel(Primative::Mesh* meshes, const std::string& name, GLenum draw_type, const bool deleteAble)
{
    std::vector<Primative::Mesh*> data = { meshes };
    return createModel(data, name, draw_type, deleteAble);
}

const Primative::Model ResourceLoader::createModel(std::vector<Primative::Mesh*>& meshes, const std::string& name, GLenum draw_type, const bool deleteAble) {
    return processMeshData(meshes, name, draw_type, deleteAble);
}

const Primative::Model ResourceLoader::getModel(const std::string& name)
{
    const unsigned s = models.size();
    auto& r = models[name];
    if (s < models.size()) {
        models.erase(name);
        return Primative::Model();
    }
    return r;
}

Primative::VertexBuffer& ResourceLoader::getBuffer(const unsigned& index)
{
    return Utils::getElement(buffers, index);
}

Materials::Material* ResourceLoader::getMaterial(const int& index)
{
    return Utils::getElement(materials, index);
}

void ResourceLoader::addMaterial(Materials::Material* mat)
{
    materials.push_back(mat);
}

const Render::Animation::Animation ResourceLoader::createAnimation(String filePath, const Render::Animation::Skeleton& skeleton)
{
    auto res = FileReaders::AssimpWrapper::loadAnimation(filePath, skeleton);

    animations.insert({ res.getName(), res });
    return res;
}

Render::Animation::Animation* ResourceLoader::getAnimation(String name)
{
    const unsigned s = animations.size();
    Render::Animation::Animation& res = animations[name];
    if (s != animations.size()) {
        animations.erase(name);
        return nullptr;
    }
    return &res;
}
