#pragma once
#include <glm.hpp>
#include <array>
#include <GL/glew.h>
namespace Materials {
	enum class Type {
		Forward, PBR
	};
	class Material {
	protected:
		Materials::Type type;
		Material();
	public:
		virtual void activateTextures(int unit = 0) const { };
		inline const Materials::Type& getType() const { return type; };
		virtual void cleanUp() = 0;
		virtual ~Material() = default;
	};
	class MatItem {
	private:
		glm::vec3 raw;
		int texId;
	public:
		inline MatItem() : raw(0), texId(-1) { };
		inline MatItem(glm::vec3 raw) : MatItem() { this->raw = raw; };
		inline MatItem(unsigned id) : MatItem() { texId = id; };

		inline const glm::vec3& getRaw() const { return raw; };
		inline const int& getId() const { return texId; };

		inline void setRaw(const glm::vec3& raw) { this->raw = raw; texId = -1; };
		inline void setId(const int& id) { this->texId = id; };
		inline const bool hasTex() const { return texId >= 0; };

		void operator()(const glm::vec3& raw);
		void operator()(const unsigned& id);
		void cleanUp();
	};
	struct Forward : public Material {
	private:
		std::array<MatItem, 3> diff_spec_norm;
	public:
		inline Forward() : diff_spec_norm{ MatItem({1, 0, 0}), MatItem({1, 1, 1}), MatItem({1, 1, 1}) }, shininess(32), Material() { Material::type = Materials::Type::Forward; };
		Forward(const MatItem& diffuse, const MatItem& specular, const MatItem& normal, float shininess);
		float shininess;
		inline MatItem& getDiffuse() { return diff_spec_norm[0]; };
		inline MatItem& getSpecular() { return diff_spec_norm[1]; };
		inline MatItem& getNormals() { return diff_spec_norm[2]; };
		inline const std::array<MatItem, 3>& getDiffSpecNorm() const  { return diff_spec_norm; };
		void activateTextures(int unit = 0) const;
		void cleanUp();
	};

	struct PBR: public Material{
	private:
		MatItem albedo;
		MatItem normal;
		MatItem metalic;
		MatItem roughness;
		MatItem ao;
		unsigned hdrMap, lbrMap, brdfTex;
	public:
		inline PBR() : albedo({ 1, 0, 0 }), normal({ 0, 1, 0 }), metalic({ 0.5, 0, 0 }), roughness({ 0.1, 0, 0 }), ao({ 1, 0, 0 }), hdrMap(0), lbrMap(0), brdfTex(0), Material() { Material::type = Materials::Type::PBR; };
		PBR(const MatItem& albedo, const MatItem& normal, const MatItem& metalic, const MatItem& roughness, const MatItem& ao);
		void activateTextures(int unit = 0) const;
		void cleanUp();

		// getters
		inline MatItem& getAlbedo()               { return albedo; };
		inline MatItem& getNormal()               { return normal; };
		inline MatItem& getMetalic()              { return metalic; };
		inline MatItem& getRoughness()            { return roughness; };
		inline MatItem& getAO()                   { return ao; };
		inline const unsigned& getHDRmap()  const { return hdrMap; };
		inline const unsigned& getIBLmap()  const { return lbrMap; };
		inline const unsigned& getBRDFtex() const { return brdfTex; };

		inline std::array<MatItem, 5> getAll() const { return std::array<MatItem, 5>{ albedo, normal, metalic, roughness, ao }; };

		// setters
		inline void setHDRmap(const unsigned& id)  { hdrMap = id; };
		inline void setIBLmap(const unsigned& id)  { lbrMap = id; };
		inline void setBRDFtex(const unsigned& id) { brdfTex= id; };


	};
};


