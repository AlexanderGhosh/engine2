#include "Material.h"

void Materials::MatItem::operator()(const glm::vec3& raw)
{
	this->setRaw(raw);
}

void Materials::MatItem::operator()(const unsigned& id)
{
	this->setId(id);
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
