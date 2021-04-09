#include "Forward.h"

Materials::Forward::Forward()
{
}

Materials::Forward::Forward(const MatItem<glm::vec3>& diffuse, const MatItem<glm::vec3>& normal, const MatItem<float>& specular)
{
}

const Materials::MatItem<glm::vec3>& Materials::Forward::getDiffuse() const
{
	return diffuse;
}

const Materials::MatItem<glm::vec3>& Materials::Forward::getNormals() const
{
	return normal;
}

const Materials::MatItem<float>& Materials::Forward::getSpecular() const
{
	return specular;
}

void Materials::Forward::activateTextures(Int startUnit) const
{
	unsigned unit = startUnit;
	diffuse.bindTexture(unit);
	normal.bindTexture(unit);
	specular.bindTexture(unit);
}

void Materials::Forward::cleanUp()
{
}
