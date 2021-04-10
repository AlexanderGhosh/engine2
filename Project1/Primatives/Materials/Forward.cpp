#include "Forward.h"
#include "MatItemBase.h"

Materials::Forward::Forward() : diffuse(nullptr), normal(nullptr), specular(nullptr)
{
}

Materials::Forward::Forward(MatItemBase<glm::vec3>* diffuse, MatItemBase<glm::vec3>* normal, MatItemBase<float>* specular) : Forward()
{
}

const Materials::MatItemBase<glm::vec3>* Materials::Forward::getDiffuse() const
{
	return diffuse;
}

const Materials::MatItemBase<glm::vec3>* Materials::Forward::getNormals() const
{
	return normal;
}

const Materials::MatItemBase<float>* Materials::Forward::getSpecular() const
{
	return specular;
}

void Materials::Forward::activateTextures(Int startUnit) const
{
	unsigned unit = startUnit;
	diffuse->tryBindTexture(unit);
	normal->tryBindTexture(unit);
	specular->tryBindTexture(unit);
}

void Materials::Forward::update(float deltaTime)
{
	diffuse->update(deltaTime);
	normal->update(deltaTime);
	specular->update(deltaTime);
}

void Materials::Forward::cleanUp()
{
	diffuse->cleanUp();
	normal->cleanUp();
	specular->cleanUp();
}
