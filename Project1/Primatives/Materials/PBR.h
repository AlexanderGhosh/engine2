#pragma once
#include "../../Utils/General.h"
#include "MaterialBase.h"

namespace Materials {
	template<class T>
	class MatItem;

	class PBR : public MaterialBase {
	private:
		MatItem<glm::vec4> albedo;
		MatItem<glm::vec3> normal;
		MatItem<float> metalic;
		MatItem<float> roughness;
		MatItem<float> ao;
		unsigned hdrMap, lbrMap, brdfTex;
	public:
		PBR();
		PBR(const MatItem<glm::vec4>& albedo, const MatItem<glm::vec3>& normal, const MatItem<float>& metalic, const MatItem<float>& roughness, const MatItem<float>& ao);
		void activateTextures(Int startUnit) const;
		void cleanUp();

		// getters
		const MatItem<glm::vec4>& getAlbedo() const;
		const MatItem<glm::vec3>& getNormal() const;
		const MatItem<float>& getMetalic() const;
		const MatItem<float>& getRoughness() const;
		const MatItem<float>& getAO() const;
		Unsigned getHDRmap() const;
		Unsigned getIBLmap() const;
		Unsigned getBRDFtex() const;

		// setters
		void setHDRmap(Unsigned id);
		void setIBLmap(Unsigned id);
		void setBRDFtex(Unsigned id);
		void setAlbedo(const MatItem<glm::vec4>& albedo);
		void setNormal(const MatItem<glm::vec3>& normal);
		void setMetalic(const MatItem<float>& metalic);
		void setRoughness(const MatItem<float>& roughness);
		void setAO(const MatItem<float>& ao);
	};
}
