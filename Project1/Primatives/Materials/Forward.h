#pragma once
#include "MaterialBase.h"

namespace Materials {
	template<class T>
	class MatItemBase;
	class Forward : public MaterialBase {
	private:
		MatItemBase<glm::vec3>* diffuse;
		MatItemBase<glm::vec3>* normal;
		MatItemBase<float>* specular;
	public:
		Forward();
		Forward(MatItemBase<glm::vec3>* diffuse, MatItemBase<glm::vec3>* normal, MatItemBase<float>* specular);
		const MatItemBase<glm::vec3>* getDiffuse() const;
		const MatItemBase<glm::vec3>* getNormals() const;
		const MatItemBase<float>* getSpecular() const;
		void activateTextures(Int startUnit) const;
		void update(float deltaTime);
		void cleanUp();
	};
}

