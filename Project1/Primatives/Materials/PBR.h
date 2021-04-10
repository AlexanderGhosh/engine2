#pragma once
#include "MaterialBase.h"

namespace Materials {
	template<class T>
	class MatItemBase;
	class PBR : public MaterialBase {
	private:
		MatItemBase<glm::vec4>* albedo;
		MatItemBase<glm::vec3>* normal;
		MatItemBase<float>* metalic;
		MatItemBase<float>* roughness;
		MatItemBase<float>* ao;
		unsigned hdrMap, lbrMap, brdfTex;
	public:
		PBR();
		PBR(MatItemBase<glm::vec4>* albedo, MatItemBase<glm::vec3>* normal, MatItemBase<float>* metalic, MatItemBase<float>* roughness, MatItemBase<float>* ao);
		void activateTextures(Int startUnit) const;
		void cleanUp();

		// getters
		const MatItemBase<glm::vec4>* getAlbedo() const;
		const MatItemBase<glm::vec3>* getNormal() const;
		const MatItemBase<float>* getMetalic() const;
		const MatItemBase<float>* getRoughness() const;
		const MatItemBase<float>* getAO() const;
		Unsigned getHDRmap() const;
		Unsigned getIBLmap() const;
		Unsigned getBRDFtex() const;

		// setters
		void setHDRmap(Unsigned id);
		void setIBLmap(Unsigned id);
		void setBRDFtex(Unsigned id);
		void setAlbedo(MatItemBase<glm::vec4>* albedo);
		void setNormal(MatItemBase<glm::vec3>* normal);
		void setMetalic(MatItemBase<float>* metalic);
		void setRoughness(MatItemBase<float>* roughness);
		void setAO(MatItemBase<float>* ao);
	};
}
