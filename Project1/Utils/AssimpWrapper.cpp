#include "AssimpWrapper.h"
#include "../Rendering/Animation/Bones.h"
#include "../Rendering/Animation/Animation.h"
#include "General.h"

const std::vector<Primative::Mesh*> FileReaders::AssimpWrapper::loadModel(std::string path)
{
    std::vector<Primative::Mesh*> meshes;
    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return meshes;
    }
    //std::string directory = path.substr(0, path.find_last_of('/'));

    Render::Animation::Skeleton skeleton;
    processNode(scene->mRootNode, scene, meshes, skeleton);
    auto animations = createAnimations(scene->mRootNode, scene, skeleton);
    return meshes;
}

void FileReaders::AssimpWrapper::processNode(aiNode* node, const aiScene* scene, std::vector<Primative::Mesh*>& meshes, Render::Animation::Skeleton& skeleton)
{
    // process all the node's meshes (if any)
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMeshVertices(mesh, scene));
        processMeshBones(mesh, meshes.back(), skeleton);

        for (Primative::Vertex& vert : meshes.back()->verts) {
            normaliseBone(vert);
        }
    }
    // then do the same for each of its children
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene, meshes, skeleton);
    }
}
/// <summary>
/// creates the mesh verices and indices no bones
/// </summary>
/// <param name="mesh"></param>
/// <param name="scene"></param>
/// <returns></returns>
Primative::Mesh* FileReaders::AssimpWrapper::processMeshVertices(aiMesh* mesh, const aiScene* scene)
{
    // data to fill
    std::vector<Primative::Vertex> vertices;
    std::vector<unsigned int> indices;
    // std::vector<Texture> textures;

    // walk through each of the mesh's vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Primative::Vertex vertex({ 0, 0, 0 });
        glm::vec3 vector(0); // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
        // positions
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.pos = vector;
        // normals
        if (mesh->HasNormals())
        {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.norm = vector;
        }
        // texture coordinates
        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            glm::vec2 vec(0);
            // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.tex = vec;
            // tangent
            /*vector.x = mesh->mTangents[i].x;
            vector.y = mesh->mTangents[i].y;
            vector.z = mesh->mTangents[i].z;
            vertex.Tangent = vector;
            // bitangent
            vector.x = mesh->mBitangents[i].x;
            vector.y = mesh->mBitangents[i].y;
            vector.z = mesh->mBitangents[i].z;
            vertex.Bitangent = vector;*/
        }
        else
            vertex.tex = glm::vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }
    // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    /*
    // process materials
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
    // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER.
    // Same applies to other texture as the following list summarizes:
    // diffuse: texture_diffuseN
    // specular: texture_specularN
    // normal: texture_normalN

    // 1. diffuse maps
    vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    // 2. specular maps
    vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    // 3. normal maps
    std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    // 4. height maps
    std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    // return a mesh object created from the extracted mesh data
    */
    return DBG_NEW Primative::Mesh(vertices, indices);
}
/// <summary>
/// populates all vertices with there bone data
/// </summary>
/// <param name="mesh"></param>
/// <param name="currentMesh"></param>
void FileReaders::AssimpWrapper::processMeshBones(aiMesh* mesh, Primative::Mesh* currentMesh, Render::Animation::Skeleton& skeleton)
{
    if (!mesh->HasBones())
        return;
    for (int i = 0; i < mesh->mNumBones; i++) {
        aiBone* bone = mesh->mBones[i];
        Render::Animation::Bone skel_bone;
        skel_bone.setName(bone->mName.C_Str());
        skel_bone.setTransformation(toMat4(bone->mOffsetMatrix));
        skeleton.addBone(skel_bone);

        aiMatrix4x4 transformation = bone->mOffsetMatrix;
        for (int j = 0; j < bone->mNumWeights; j++) {
            aiVertexWeight weight = bone->mWeights[j];
            Primative::Vertex& currVertex = currentMesh->verts[weight.mVertexId];
            addBone(currVertex, weight);
        }
    }
    return;
}
/// <summary>
/// replaces the bone with the smallest weighting
/// </summary>
/// <param name="vertex"></param>
/// <param name="weighting"></param>
void FileReaders::AssimpWrapper::addBone(Primative::Vertex& vertex, const aiVertexWeight& weighting)
{
    Render::Animation::BoneDetails& min_bone = vertex.boneDetails[0];
    float min_weight = min_bone.boneWeight;
    for (short i = 0; i < MAX_BONE_WEIGHTS; i++) {
        Render::Animation::BoneDetails& bone = vertex.boneDetails[i];
        if (min_weight > bone.boneWeight) {
            min_weight = bone.boneWeight;
            min_bone = bone;
        }
    }
    min_bone.boneIndex = weighting.mVertexId;
    min_bone.boneWeight = weighting.mWeight;
}

void FileReaders::AssimpWrapper::normaliseBone(Primative::Vertex& vertex)
{
    float sum = 0;
    for (const auto& b : vertex.boneDetails) {
        sum += powf(b.boneWeight, 2.0f);
    }
    sum /= static_cast<float>(vertex.boneDetails.size());
    for (auto& b : vertex.boneDetails) {
        b.boneWeight /= sum;
    }
}

std::vector<Render::Animation::Animation> FileReaders::AssimpWrapper::createAnimations(aiNode* rootNode, const aiScene* scene, const Render::Animation::Skeleton& skeleton)
{
    std::vector<Render::Animation::Animation> animations;
    glm::mat4 globalInverseTransformation = glm::inverse(toMat4(scene->mRootNode->mTransformation));
    // Process all animations
    for (int i = 0; i < scene->mNumAnimations; i++) {
        aiAnimation* aiAnimation = scene->mAnimations[i];
        int maxFrames = calcAnimationFrameCount(aiAnimation);

        Render::Animation::Animation animation(aiAnimation->mName.C_Str(), aiAnimation->mDuration);

            //aiAnimation.mName().dataString(), frames, aiAnimation.mDuration();

        for (int j = 0; j < maxFrames; j++) {
            Render::Animation::KeyFrame animatedFrame;
            const int s = skeleton.getBones().size();
            animatedFrame.translations.reserve(s);
            animatedFrame.translations.resize(s);
            processAnimNode(aiAnimation, rootNode, toMat4(scene->mRootNode->mTransformation), globalInverseTransformation, animatedFrame, j, skeleton.getBones());
            animation.addKeyFrame(animatedFrame);
        }
        animations.push_back(animation);
    }
    return animations;
}

void FileReaders::AssimpWrapper::processAnimNode(const aiAnimation* anim, aiNode* node, 
    const glm::mat4& parentsTransform, const glm::mat4& globalInverseTransform,
    Render::Animation::KeyFrame& keyFrame, int frame, const std::vector<Render::Animation::Bone>& bones)
{
    const std::string nodeName = node->mName.C_Str();
    glm::mat4 nodeTransform = toMat4(node->mTransformation);
    aiNodeAnim* animNode = nullptr;
    for (int i = 0; i < anim->mNumChannels; i++) {
        auto a_node = anim->mChannels[i];
        if (a_node->mNodeName.C_Str() == nodeName) {
            animNode = a_node;
            break;
        }
    }
    if (animNode) {
        // build matrix from animNode asine to nodeTransform
        nodeTransform = nodeTransform;
    }
    glm::mat4 nodeGlobalTransform = parentsTransform * nodeTransform;
    // get all bones whos name == nodeName then do tranformation stuff
    for (int i = 0; i < bones.size(); i++) {
        const Render::Animation::Bone& bone = bones[i];
        if (bone.getName() == nodeName) {
            glm::mat4 boneTransform = globalInverseTransform * nodeGlobalTransform * bone.getTransformation();
            keyFrame.translations[i] = boneTransform;
        }
    }

    glm::mat4 global = parentsTransform * nodeTransform;

    // for each child of node reccersive
    for (int i = 0; i < node->mNumChildren; i++) {
        aiNode* n = node->mChildren[i];
        processAnimNode(anim, n, global, globalInverseTransform, keyFrame, frame, bones);
    }

    
}

int FileReaders::AssimpWrapper::calcAnimationFrameCount(const aiAnimation* animtion)
{
    int maxFrames = 0;
    for (int i = 0; i < animtion->mNumChannels; i++) {
        aiNodeAnim* aiNodeAnim = animtion->mChannels[i];
        int numFrames = fmaxf(
            fmaxf(aiNodeAnim->mNumPositionKeys, aiNodeAnim->mNumScalingKeys), 
            aiNodeAnim->mNumRotationKeys);
        maxFrames = fmaxf(maxFrames, numFrames);
    }
    return maxFrames;
}

const glm::mat4 FileReaders::AssimpWrapper::toMat4(const aiMatrix4x4& matrix)
{
    return glm::mat4(1);
}
