#pragma once
#include "../../Utils/General.h"
#include "MaterialBase.h"

namespace Materials {
	class Forward : public MaterialBase {
	private:
		MatItem<glm::vec3> diffuse;
		MatItem<glm::vec3> normal;
		MatItem<float> specular;
	public:
		Forward();
		Forward(const MatItem<glm::vec3>& diffuse, const MatItem<glm::vec3>& normal, const MatItem<float>& specular);
		const MatItem<glm::vec3>& getDiffuse() const;
		const MatItem<glm::vec3>& getNormals() const;
		const MatItem<float>& getSpecular() const;
		void activateTextures(Int startUnit) const;
		void cleanUp();
	};
}

