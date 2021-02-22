#include "Bones.h"

Render::Animation::Bone::Bone() : name(""), transformation()
{
}

const std::string& Render::Animation::Bone::getName() const
{
    return name;
}

void Render::Animation::Bone::setName(const std::string& name)
{
    this->name = name;
}

const glm::mat4& Render::Animation::Bone::getTransformation() const
{
    return transformation;
}

void Render::Animation::Bone::setTransformation(const glm::mat4& matrix)
{
    transformation = matrix;
}

Render::Animation::BoneDetails::BoneDetails() : boneIndex(0), boneWeight(0)
{
}

inline const Render::Animation::Bone& Render::Animation::BoneDetails::getBone() const
{
    //return (*parent)[boneIndex];
    return {};
}

Render::Animation::Skeleton::Skeleton() : bones()
{
}

Render::Animation::Bone& Render::Animation::Skeleton::operator[](const int& index)
{
    return bones[index];
}
Render::Animation::Bone& Render::Animation::Skeleton::operator[](const std::string& name)
{
    for (Bone& bone : bones)
        if (bone.getName() == name)
            return bone;
    bones.push_back({});
    return bones.back();
}

Render::Animation::Skeleton Render::Animation::Skeleton::operator+(const Skeleton& skel) const
{
    Skeleton res;
    res.bones = this->bones;
    res.bones.insert(res.bones.end(), skel.bones.begin(), skel.bones.end());
    return res;
}

void Render::Animation::Skeleton::addBone(const Bone& bone)
{
    bones.push_back(bone);
}

const std::vector<Render::Animation::Bone>& Render::Animation::Skeleton::getBones() const
{
    return bones;
}

bool Render::Animation::Skeleton::hasBones() const
{
    return !bones.empty();
}

void Render::Animation::Skeleton::cleanUp()
{
    bones.clear();
}
