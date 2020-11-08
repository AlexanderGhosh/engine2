#pragma once
#include <vector>
#include "../Primatives/Buffers.h"
#include "../Utils/ResourceLoader.h"
#include "../Primatives/Mesh.h"
#include "Animation/Animation.h"
#include "../Componets/Componets.h"
#include "../Primatives/Material.h"
namespace Render {
	class RenderMesh : public Animation::Animation, public Componet::Base // special case of animation with only one frame
	{
	public:
		RenderMesh();
		virtual void update();
		void addMesh(std::vector<Primative::VertexBuffer*>& buffers, const GLenum draw_type = GL_TRIANGLES);
		inline void setMaterial(Materials::Base* mat) { material = mat; };
	private:
		std::vector<Primative::VertexBuffer*> buffers;
		GameObject* parent;
		Materials::Base* material;
	};
}

