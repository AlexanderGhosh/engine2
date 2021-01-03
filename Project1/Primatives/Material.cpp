#include "Material.h"

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

void Materials::Forward::activateTextures() const
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

void Materials::PBR::activateTextures() const
{
	short i = 0;
	for (const Materials::MatItem& item : this->getAll()) {
		if (!item.hasTex()) continue;
		glActiveTexture(GL_TEXTURE0 + i++);
		glBindTexture(GL_TEXTURE_2D, item.getId());
	}
}

void Materials::PBR::cleanUp()
{
}
