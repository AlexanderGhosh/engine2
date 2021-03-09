#pragma once
#include <string>
#include <iostream>
#include <glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <GL/glew.h>
#include <list>
#include <tuple>
#include "General.h"

namespace Primative {
    class Mesh;
    class Vertex;
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
        static const std::tuple<std::vector<Primative::Mesh*>, std::vector<Render::Animation::Animation>, Render::Animation::Skeleton> loadModel(String path);
        static const Render::Animation::Animation loadAnimation(String path, const Render::Animation::Skeleton& skeleton);
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
            Render::Animation::KeyFrame& keyFrame, const float frame, const std::vector<Render::Animation::Bone>& bones);
        static int calcAnimationFrameCount(const aiAnimation* animtion);
        static glm::mat4 buildMatrix(const aiNodeAnim* node, float frame);

        static const glm::mat4 toMat4(const aiMatrix4x4& matrix);
        static const glm::vec3 toVec3(const aiVector3D& vector);
        static const glm::quat toQuat(const aiQuaternion& quat);
    };
};



