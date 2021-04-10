#include "PBR.h"
#include "MatItemBase.h"

Materials::PBR::PBR() : MaterialBase(), albedo(nullptr), normal(nullptr), metalic(nullptr), roughness(nullptr), ao(nullptr), brdfTex(0), lbrMap(0), hdrMap(0)
{
}

Materials::PBR::PBR(MatItemBase<glm::vec4>* albedo, MatItemBase<glm::vec3>* normal, 
	MatItemBase<float>* metalic, MatItemBase<float>* roughness, MatItemBase<float>* ao) : PBR()
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

	albedo->tryBindTexture(unit);
	normal->tryBindTexture(unit);
	metalic->tryBindTexture(unit);
	roughness->tryBindTexture(unit);
	ao->tryBindTexture(unit);

	glActiveTexture(GL_TEXTURE0 + unit++);
	glBindTexture(GL_TEXTURE_CUBE_MAP, hdrMap);
	glActiveTexture(GL_TEXTURE0 + unit++);
	glBindTexture(GL_TEXTURE_CUBE_MAP, lbrMap);
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, brdfTex);
}

void Materials::PBR::cleanUp()
{
	albedo->cleanUp();
	normal->cleanUp();
	metalic->cleanUp();
	roughness->cleanUp();
	ao->cleanUp();
}

void Materials::PBR::update(float deltaTime)
{
	albedo->update(deltaTime);
	normal->update(deltaTime);
	metalic->update(deltaTime);
	roughness->update(deltaTime);
	ao->update(deltaTime);
}

const Materials::MatItemBase<glm::vec4>* Materials::PBR::getAlbedo() const
{
	return albedo;
}

const Materials::MatItemBase<glm::vec3>* Materials::PBR::getNormal() const
{
	return normal;
}

const Materials::MatItemBase<float>* Materials::PBR::getMetalic() const
{
	return metalic;
}

const Materials::MatItemBase<float>* Materials::PBR::getRoughness() const
{
	return roughness;
}

const Materials::MatItemBase<float>* Materials::PBR::getAO() const
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

void Materials::PBR::setAlbedo(MatItemBase<glm::vec4>* albedo)
{
	this->albedo = albedo;
}

void Materials::PBR::setNormal(MatItemBase<glm::vec3>* normal)
{
	this->normal = normal;
}

void Materials::PBR::setMetalic(MatItemBase<float>* metalic)
{
	this->metalic = metalic;
}

void Materials::PBR::setRoughness(MatItemBase<float>* roughness)
{
	this->roughness = roughness;
}

void Materials::PBR::setAO(MatItemBase<float>* ao)
{
	this->ao = ao;
}
