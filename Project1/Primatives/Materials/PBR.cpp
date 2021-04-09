#include "PBR.h"

Materials::PBR::PBR() : MaterialBase(), albedo(), normal(), metalic(), roughness(), ao(), brdfTex(0), lbrMap(0), hdrMap(0)
{
}

Materials::PBR::PBR(const MatItem<glm::vec4>& albedo, const MatItem<glm::vec3>& normal, 
	const MatItem<float>& metalic, const MatItem<float>& roughness, const MatItem<float>& ao) : PBR()
{
	this->albedo = albedo;
	this->normal = normal;
	this->metalic = metalic;
	this->roughness = roughness;
	this->ao = ao;
}

void Materials::PBR::activateTextures(Int startUnit) const
{
	unsigned unit = startUnit;

	albedo.bindTexture(unit);
	normal.bindTexture(unit);
	metalic.bindTexture(unit);
	roughness.bindTexture(unit);
	ao.bindTexture(unit);

	glActiveTexture(GL_TEXTURE0 + unit++);
	glBindTexture(GL_TEXTURE_CUBE_MAP, hdrMap);
	glActiveTexture(GL_TEXTURE0 + unit++);
	glBindTexture(GL_TEXTURE_CUBE_MAP, lbrMap);
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, brdfTex);
}

void Materials::PBR::cleanUp()
{
}

const Materials::MatItem<glm::vec4>& Materials::PBR::getAlbedo() const
{
	return albedo;
}

const Materials::MatItem<glm::vec3>& Materials::PBR::getNormal() const
{
	return normal;
}

const Materials::MatItem<float>& Materials::PBR::getMetalic() const
{
	return metalic;
}

const Materials::MatItem<float>& Materials::PBR::getRoughness() const
{
	return roughness;
}

const Materials::MatItem<float>& Materials::PBR::getAO() const
{
	return ao;
}

Unsigned Materials::PBR::getHDRmap() const
{
	return hdrMap;
}

Unsigned Materials::PBR::getIBLmap() const
{
	return lbrMap;
}

Unsigned Materials::PBR::getBRDFtex() const
{
	return brdfTex;
}

void Materials::PBR::setHDRmap(Unsigned id)
{
	hdrMap = id;
}

void Materials::PBR::setIBLmap(Unsigned id)
{
	lbrMap = id;
}

void Materials::PBR::setBRDFtex(Unsigned id)
{
	brdfTex = id;
}

void Materials::PBR::setAlbedo(const MatItem<glm::vec4>& albedo)
{
	this->albedo = albedo;
}

void Materials::PBR::setNormal(const MatItem<glm::vec3>& normal)
{
	this->normal = normal;
}

void Materials::PBR::setMetalic(const MatItem<float>& metalic)
{
	this->metalic = metalic;
}

void Materials::PBR::setRoughness(const MatItem<float>& roughness)
{
	this->roughness = roughness;
}

void Materials::PBR::setAO(const MatItem<float>& ao)
{
	this->ao = ao;
}
