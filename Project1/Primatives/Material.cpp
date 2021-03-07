#include "Material.h"
#include "../Utils/ResourceLoader.h"

Materials::Material::Material() : type()
{
	ResourceLoader::addMaterial(this);
}


void Materials::MatItem::operator()(const glm::vec3& raw)
{
	this->setRaw(raw);
}

void Materials::MatItem::operator()(const unsigned& id)
{
	this->setId(id);
}

void Materials::MatItem::cleanUp()
{
}

Materials::Forward::Forward(const MatItem& diffuse, const MatItem& specular, const MatItem& normal, float shininess) :
	diff_spec_norm{ diffuse, specular, normal }, shininess(shininess), Material()
{
	Material::type = Materials::Type::Forward;
}

void Materials::Forward::activateTextures(int unit) const
{
	for (short i = 0; i < diff_spec_norm.size(); i++) {
		const auto& d_s = this->diff_spec_norm[i];
		if (!d_s.hasTex()) continue;
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, d_s.getId());
	}
}

void Materials::Forward::cleanUp()
{
}

Materials::PBR::PBR(const MatItem& albedo, const MatItem& normal, const MatItem& metalic, const MatItem& roughness, const MatItem& ao) :
	albedo(albedo), normal(normal), metalic(metalic), roughness(roughness), ao(ao), Material()
{
	Material::type = Materials::Type::PBR;
}

void Materials::PBR::activateTextures(int unit) const
{
	for (const Materials::MatItem& item : getAll()) {
		if (!item.hasTex()) continue;
		glActiveTexture(GL_TEXTURE0 + unit++);
		glBindTexture(GL_TEXTURE_2D, item.getId());
	}
	glActiveTexture(GL_TEXTURE0 + unit++);
	glBindTexture(GL_TEXTURE_CUBE_MAP, hdrMap);
	glActiveTexture(GL_TEXTURE0 + unit++);
	glBindTexture(GL_TEXTURE_CUBE_MAP, getIBLmap());
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, getBRDFtex());
}

void Materials::PBR::cleanUp()
{
}