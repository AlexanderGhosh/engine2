#include "Model.h"
#include "../Rendering/Shading/Manager.h"

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

void Primative::Model::cleanUp()
{
	name = "";
	buffers.clear();
	skeleton.cleanUp();
}

void Primative::Model::addBuffer(Unsigned bufferIndex)
{
	buffers.push_back(bufferIndex);
}

const std::vector<unsigned>& Primative::Model::getBuffers() const
{
	return buffers;
}

bool Primative::Model::hasMesh() const
{
	return !buffers.empty();
}
