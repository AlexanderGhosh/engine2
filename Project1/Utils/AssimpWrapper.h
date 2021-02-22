#pragma once
#include <string>
#include <iostream>
#include <glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../Primatives/Vertex.h"
#include "../Primatives/Mesh.h"
#include <GL/glew.h>
#include <list>
#include <tuple>

namespace Primative {
    struct Mesh;
}; 
namespace Render {
    namespace Animation {
        class Skeleton;
        class Bone;
        class BoneDetails;
        class Animation;
        class KeyFrame;
    }
}

namespace FileReaders {
    class AssimpWrapper
    {
    public:
        static const std::tuple<std::vector<Primative::Mesh*>, std::vector<Render::Animation::Animation>, Render::Animation::Skeleton> loadModel(std::string path);
    private:
        // mesh for the most part
        static void processNode(aiNode* node, const aiScene* scene, std::vector<Primative::Mesh*>& meshes, Render::Animation::Skeleton& skeleton);
        static Primative::Mesh* processMeshVertices(aiMesh* mesh, const aiScene* scene);
        // bones
        static void processMeshBones(aiMesh* mesh, Primative::Mesh* currentMesh, Render::Animation::Skeleton& skeleton, const aiScene* scene);
        static void addBone(Primative::Vertex& vertex, const aiVertexWeight& weighting, Unsigned boneId);
        static void normaliseBone(Primative::Vertex& vertex);
        // animations
        static std::vector<Render::Animation::Animation> createAnimations(aiNode* rootNode, const aiScene* scene, const Render::Animation::Skeleton& skeleton);
        static void processAnimNode(const aiAnimation* anim, const aiNode* node,
            const glm::mat4& parentsTransform, const glm::mat4& globalInverseTransform,
            Render::Animation::KeyFrame& keyFrame, const int frame, const std::vector<Render::Animation::Bone>& bones);
        static int calcAnimationFrameCount(const aiAnimation* animtion);
        static glm::mat4 buildMatrix(const aiNodeAnim* node, float frame);

        static const glm::mat4 toMat4(const aiMatrix4x4& matrix);
        static const glm::vec3 toVec3(const aiVector3D& vector);
        static const glm::quat toQuat(const aiQuaternion& quat);

        /*std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
        {
            std::vector<Texture> textures;
            for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
            {
                aiString str;
                mat->GetTexture(type, i, &str);
                // check if texture was loaded before and if so, continue to next iteration: skip loading a DBG_NEW texture
                bool skip = false;
                for (unsigned int j = 0; j < textures_loaded.size(); j++)
                {
                    if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
                    {
                        textures.push_back(textures_loaded[j]);
                        skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                        break;
                    }
                }
                if (!skip)
                {   // if texture hasn't been loaded already, load it
                    Texture texture;
                    texture.id = TextureFromFile(str.C_Str(), this->directory);
                    texture.type = typeName;
                    texture.path = str.C_Str();
                    textures.push_back(texture);
                    textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
                }
            }
            return textures;
        }*/
        
        /*unsigned int TextureFromFile(const char* path, const string& directory, bool gamma)
        {
            string filename = string(path);
            filename = directory + '/' + filename;

            unsigned int textureID;
            glGenTextures(1, &textureID);

            int width, height, nrComponents;
            unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
            if (data)
            {
                GLenum format;
                if (nrComponents == 1)
                    format = GL_RED;
                else if (nrComponents == 3)
                    format = GL_RGB;
                else if (nrComponents == 4)
                    format = GL_RGBA;

                glBindTexture(GL_TEXTURE_2D, textureID);
                glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                stbi_image_free(data);
            }
            else
            {
                std::cout << "Texture failed to load at path: " << path << std::endl;
                stbi_image_free(data);
            }

            return textureID;
        }*/
    };
};



