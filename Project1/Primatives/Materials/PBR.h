#pragma once
#include "MaterialBase.h"

namespace Materials {
	template<class T>
	class MatItemBase;
	class PBR : public MaterialBase {
	private:
		MatItemBase<glm::vec4>* albedo;
		MatItemBase<glm::vec3>* normal;
		MatItemBase<glm::vec3>* emission;
		MatItemBase<float>* metalic;
		MatItemBase<float>* roughness;
		MatItemBase<float>* ao;
	public:
		PBR();
		PBR(MatItemBase<glm::vec4>* albedo, MatItemBase<glm::vec3>* normal, MatItemBase<glm::vec3>* emission, MatItemBase<float>* metalic, MatItemBase<float>* roughness, MatItemBase<float>* ao);
		void activateTextures(Int startUnit) const;
		void cleanUp();
		void update(float deltaTime);

		// getters
		const MatItemBase<glm::vec4>* getAlbedo() const;
		const MatItemBase<glm::vec3>* getNormal() const;
		const MatItemBase<glm::vec3>* getEmission() const;
		const MatItemBase<float>* getMetalic() const;
		const MatItemBase<float>* getRoughness() const;
		const MatItemBase<float>* getAO() const;

		// setters
		void setAlbedo(MatItemBase<glm::vec4>* albedo);
		void setNormal(MatItemBase<glm::vec3>* normal);
		void setEmission(MatItemBase<glm::vec3>* emission);
		void setMetalic(MatItemBase<float>* metalic);
		void setRoughness(MatItemBase<float>* roughness);
		void setAO(MatItemBase<float>* ao);
		void setRepeatValue(Float mul);
	};
}
