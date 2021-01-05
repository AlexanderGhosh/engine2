#pragma once
#include <vector>
#include "../Primatives/Buffers.h"
#include "../Utils/ResourceLoader.h"
#include "../Primatives/Mesh.h"
#include "Animation/Animation.h"
#include "../Componets/Componets.h"
#include "../Primatives/Material.h"
namespace Render {
	class RenderMesh : public Animation::Animation, public Component::ComponetBase // special case of animation with only one frame
	{
	public:
		RenderMesh();
		virtual void update();
		void addBuffers(std::vector<Primative::VertexBuffer*>& buffers, const GLenum draw_type = GL_TRIANGLES);
		inline void setMaterial(Materials::Material* mat) { material = mat; };
		inline Component::Type getType() const {
			return Component::Type::RenderMesh;
		}
		void cleanUp();
		Materials::Material* material;
	private:
		std::vector<Primative::VertexBuffer*> buffers;
		GameObject* parent;
	};
}

