#include "Model.h"

Primative::Model::Model() : name(""), buffers(), skeleton()
{
}

const std::string& Primative::Model::getName() const
{
	return name;
}

void Primative::Model::setName(const std::string& name)
{
	this->name = name;
}

const Render::Animation::Skeleton& Primative::Model::getSkeleton() const
{
	return skeleton;
}

void Primative::Model::setSkeleton(const Render::Animation::Skeleton& skeleton)
{
	this->skeleton = skeleton;
}
