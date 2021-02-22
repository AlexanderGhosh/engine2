#pragma once
#include <vector>
#include "../Primatives/Buffers.h"
#include "../Utils/ResourceLoader.h"
#include "../Primatives/Mesh.h"
#include "../Primatives/Model.h"
#include "Animation/Animation.h"
#include "../Componets/Componets.h"
#include "../Primatives/Material.h"
namespace Component {
	class Animated;
	class RenderMesh : public Component::ComponetBase // special case of animation with only one frame
	{
	public:
		RenderMesh();
		~RenderMesh() = default;
		virtual void update();
		void setModel(const Primative::Model model, const GLenum draw_type = GL_TRIANGLES);
		inline void setMaterial(const std::vector<Materials::Material*>& indices) { materials = std::vector<Materials::Material*>(indices.begin(), indices.end()); };
		inline Component::Type getType() const {
			return Component::Type::RenderMesh;
		}

		void setAnimatedComp(Component::Animated* comp);
		void cleanUp();

		const Primative::Model& getModel() const;

		RenderMesh copy() const;
	private:
		// index of buffer in the resorce loader
		std::vector<Materials::Material*> materials;
		Primative::Model model;
		Animated* animatedComponet;
	};
}

