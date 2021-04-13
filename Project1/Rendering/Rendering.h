#pragma once
#include <GL/glew.h>
#include "../Componets/Componets.h"
#include "../Utils/General.h"
#include "../Primatives/Model.h"

namespace Materials {
	class MaterialBase;
}
namespace Component {
	class Animated;
	class RenderMesh : public ComponetBase
	{
	public:
		RenderMesh();
		~RenderMesh() = default;
		void render(float deltaTime);
		void setModel(const Primative::Model model, const GLenum draw_type = GL_TRIANGLES);
		void setMaterial(Materials::MaterialBase* material);
		void setMaterialTo(Materials::MaterialBase* material, String meshName);

		inline Component::Type getType() const {
			return Component::Type::RenderMesh;
		}

		void setTransparent(bool transparent);
		const bool getTransparent() const;

		void setAnimatedComp(Component::Animated* comp);
		void cleanUp();

		const Primative::Model& getModel() const;

		RenderMesh copy() const;
	private:
		// index of buffer in the resorce loader
		std::vector<Materials::MaterialBase*> materials;
		Primative::Model model;
		Animated* animatedComponet;
		bool isTransparent;
	};
}

