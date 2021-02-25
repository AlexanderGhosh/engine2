#include "AssimpWrapper.h"
#include "../Rendering/Animation/Bones.h"
#include "../Rendering/Animation/Animation.h"
#include "General.h"
#include <gtc/type_ptr.hpp>

const std::tuple<std::vector<Primative::Mesh*>, std::vector<Render::Animation::Animation>, Render::Animation::Skeleton> FileReaders::AssimpWrapper::loadModel(std::string path)
{
    std::vector<Primative::Mesh*> meshes;
    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_FixInfacingNormals | aiProcess_OptimizeGraph | aiProcess_OptimizeMeshes);

    if (NOT scene OR scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE OR NOT scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return { meshes, { }, { } };
    }
    //std::string directory = path.substr(0, path.find_last_of('/'));

    Render::Animation::Skeleton skeleton;
    processNode(scene->mRootNode, scene, meshes, skeleton);
    auto animations = createAnimations(scene->mRootNode, scene, skeleton);
    return { meshes, animations, skeleton };
}

void FileReaders::AssimpWrapper::processNode(aiNode* node, const aiScene* scene, std::vector<Primative::Mesh*>& meshes, Render::Animation::Skeleton& skeleton)
{
    // process all the node's meshes (if any)
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMeshVertices(mesh, scene));
        processMeshBones(mesh, meshes.back(), skeleton, scene);

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
    return DBG_NEW Primative::Mesh(vertices, indices, mesh->mName.C_Str());
}
/// <summary>
/// populates all vertices with there bone data
/// </summary>
/// <param name="mesh"></param>
/// <param name="currentMesh"></param>
void FileReaders::AssimpWrapper::processMeshBones(aiMesh* mesh, Primative::Mesh* currentMesh, Render::Animation::Skeleton& skeleton, const aiScene* scene)
{
    if (!mesh->HasBones())
        return;
    for (int i = 0; i < mesh->mNumBones; i++) {
        aiBone* bone = mesh->mBones[i];
        Render::Animation::Bone skel_bone;
        skel_bone.setName(bone->mName.C_Str());
        skel_bone.setTransformation(toMat4(bone->mOffsetMatrix));
        skeleton.addBone(skel_bone);

        for (int j = 0; j < bone->mNumWeights; j++) {
            aiVertexWeight weight = bone->mWeights[j];
            assert(weight.mVertexId < currentMesh->verts.size());
            Primative::Vertex& currVertex = currentMesh->verts[weight.mVertexId];
            addBone(currVertex, weight, skeleton.getBones().size()-1);
        }
    }
    return;
}
/// <summary>
/// replaces the bone with the smallest weighting
/// </summary>
/// <param name="vertex"></param>
/// <param name="weighting"></param>
void FileReaders::AssimpWrapper::addBone(Primative::Vertex& vertex, const aiVertexWeight& weighting, Unsigned boneId)
{
    float min_weight = vertex.weights[0];
    short j = 0;
    for (short i = 1; i < MAX_BONE_WEIGHTS; i++) {
        if (vertex.weights[i] < min_weight) {
            min_weight = vertex.weights[i];
            j = i;
        }
    }
    vertex.ids[j] = boneId;
    vertex.weights[j] = weighting.mWeight;
}

void FileReaders::AssimpWrapper::normaliseBone(Primative::Vertex& vertex)
{
    float sum = 0;
    for (char i = 0; i < 4; i++) {
        sum += vertex.weights[i];
    }
    if (sum == 0) {
        sum = 1;
    }
    for (char i = 0; i < 4; i++) {
        vertex.weights[i] /= sum;
    }
}

std::vector<Render::Animation::Animation> FileReaders::AssimpWrapper::createAnimations(aiNode* rootNode, const aiScene* scene, const Render::Animation::Skeleton& skeleton)
{
    std::vector<Render::Animation::Animation> animations;
    if (!scene->HasAnimations())
        return animations;
    glm::mat4 globalInverseTransformation = toMat4(scene->mRootNode->mTransformation.Inverse());
    // Process all animations
    for (int i = 0; i < scene->mNumAnimations; i++) {
        aiAnimation* currentAnimation = scene->mAnimations[i];
        int maxFrames = calcAnimationFrameCount(currentAnimation);

        Render::Animation::Animation animation(currentAnimation->mName.C_Str(), currentAnimation->mDuration, currentAnimation->mTicksPerSecond);

            //currentAnimation.mName().dataString(), frames, currentAnimation.mDuration();

        const int s = skeleton.getBones().size();
        for (int j = 0; j < maxFrames; j++) {
            Render::Animation::KeyFrame animatedFrame;
            animatedFrame.translations.reserve(s);
            animatedFrame.translations.resize(s);
            for (int i = 0; i < s; i++) {
                animatedFrame.translations[i] = glm::mat4(1);
            }
            //toMat4(scene->mRootNode->mTransformation)
            animatedFrame.timeStamp = (animation.getDuration() / static_cast<float>(maxFrames - 1)) * j;
            processAnimNode(currentAnimation, rootNode, toMat4(scene->mRootNode->mTransformation), globalInverseTransformation, animatedFrame, j, skeleton.getBones());
            animation.addKeyFrame(animatedFrame);
        }
        animations.push_back(animation);
    }
    /*bool t = true;
    for (int i = 0; i < animations[0].getFrameCount(); i++) {
        for (int j = i + 1; j < animations[0].getFrameCount(); j++) {
            t = t AND animations[0].getFrameABS(i).translations != animations[0].getFrameABS(j).translations;
        }
    }
    assert(NOT t);*/
    return animations;
}

void FileReaders::AssimpWrapper::processAnimNode(const aiAnimation* anim, const aiNode* node, 
    const glm::mat4& parentsTransform, const glm::mat4& globalInverseTransform,
    Render::Animation::KeyFrame& keyFrame, const float frame, const std::vector<Render::Animation::Bone>& bones)
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
        nodeTransform = buildMatrix(animNode, keyFrame.timeStamp);
    }
    glm::mat4 nodeGlobalTransform = parentsTransform * nodeTransform;
    // get all bones whos name == nodeName then do tranformation stuff
    glm::mat4 git = glm::inverse(toMat4(node->mTransformation));
    for (int i = 0; i < bones.size(); i++) {
        const Render::Animation::Bone& bone = bones[i];
        if (bone.getName() == nodeName) {
            glm::mat4 boneTransform = globalInverseTransform * nodeGlobalTransform * bone.getTransformation();
            // glm::mat4 boneTransform = bone.getTransformation() * nodeGlobalTransform * globalInverseTransform;
            keyFrame.translations[i] = boneTransform;
            break;
        }
    }

    // for each child of node reccersive
    for (int i = 0; i < node->mNumChildren; i++) {
        processAnimNode(anim, node->mChildren[i], nodeGlobalTransform, globalInverseTransform, keyFrame, frame, bones);
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

glm::mat4 FileReaders::AssimpWrapper::buildMatrix(const aiNodeAnim* node, float frame)
{
    int f = static_cast<int>(frame);
    
    // position
    glm::mat4 translation(1), rotation(1), scale(1);
    const float b = Utils::round(frame, 3);
    for (unsigned i = 0; i < node->mNumPositionKeys; i++) {
        auto key = node->mPositionKeys[i];
        float a = Utils::round(key.mTime, 3);
        if (a <= b) {
            translation = glm::translate(glm::mat4(1), toVec3(node->mPositionKeys[i].mValue));
            rotation = glm::mat4_cast(toQuat(node->mRotationKeys[i].mValue));
            scale = glm::scale(glm::mat4(1), toVec3(node->mScalingKeys[i].mValue));
        }
        else {
            break;
        }
    }
    /*if (node->mNumPositionKeys == 1 OR true) {
        translation = glm::translate(translation, toVec3(node->mPositionKeys[f].mValue));
    }
    /*int numPositions = node->mNumPositionKeys;
    if (numPositions > 0) {
        aiVecKey = positionKeys[numPositions - 1];
        for (int i = 0; i < numPositions; i++) {
            if (positionKeys[i].mTime == static_cast<double>(frame)) {
                aiVecKey = positionKeys[i];
                break;
            }
        }
        //aiVecKey = positionKeys[static_cast<int>(fminf(numPositions - 1, frame))];
        vec = aiVecKey.mValue;
        nodeTransform = glm::translate(nodeTransform, glm::vec3(vec.x, vec.y, vec.z));
    }
    // rotation
    glm::mat4 rotation(1);
    if (node->mNumRotationKeys == 1 OR true) {
        rotation = glm::mat4_cast(toQuat(node->mRotationKeys[f].mValue));
    }
    /*int numRotations = node->mNumRotationKeys;
    if (numRotations > 0) {
        aiQuatKey quatKey = rotationKeys[numRotations - 1];
        for (int i = 0; i < numRotations; i++) {
            if (rotationKeys[i].mTime == static_cast<double>(frame)) {
                quatKey = rotationKeys[i];
                break;
            }
        }
        // aiQuatKey quatKey = rotationKeys[static_cast<int>(fminf(numRotations - 1, frame))];
        aiQuaternion aiQuat = quatKey.mValue;
        glm::quat quat = glm::quat(aiQuat.w, aiQuat.x, aiQuat.y, aiQuat.z);
        // nodeTransform = Utils::rotate(nodeTransform, quat);

        nodeTransform = nodeTransform * glm::mat4(quat);
    }
    // scaling
    glm::mat4 scale(1);
    if (node->mNumScalingKeys == 1 OR true) {
        scale = glm::scale(scale, toVec3(node->mScalingKeys[f].mValue));
    }*/
    /*int numScalingKeys = node->mNumScalingKeys;
    if (numScalingKeys > 0) {
        aiVecKey = scalingKeys[numScalingKeys - 1];
        for (int i = 0; i < numScalingKeys; i++) {
            if (scalingKeys[i].mTime == static_cast<double>(frame)) {
                aiVecKey = scalingKeys[i];
                break;
            }
        }
        // aiVecKey = scalingKeys[static_cast<int>(fminf(numScalingKeys - 1, frame))];
        vec = aiVecKey.mValue;
        nodeTransform = glm::scale(nodeTransform, glm::vec3(vec.x, vec.y, vec.z));
    }*/
    return  translation * rotation * scale;
}

const glm::mat4 FileReaders::AssimpWrapper::toMat4(const aiMatrix4x4& matrix)
{
    glm::mat4 res(1);
    res[0][0] = matrix.a1; res[1][0] = matrix.a2;
    res[2][0] = matrix.a3; res[3][0] = matrix.a4;
    res[0][1] = matrix.b1; res[1][1] = matrix.b2;
    res[2][1] = matrix.b3; res[3][1] = matrix.b4;
    res[0][2] = matrix.c1; res[1][2] = matrix.c2;
    res[2][2] = matrix.c3; res[3][2] = matrix.c4;
    res[0][3] = matrix.d1; res[1][3] = matrix.d2;
    res[2][3] = matrix.d3; res[3][3] = matrix.d4;
    return res;
}

const glm::vec3 FileReaders::AssimpWrapper::toVec3(const aiVector3D& vector)
{
    return glm::vec3(vector.x, vector.y, vector. z);
}

const glm::quat FileReaders::AssimpWrapper::toQuat(const aiQuaternion& quat)
{
    return glm::normalize(glm::quat(quat.w, quat.x, quat.y, quat.z));
}
