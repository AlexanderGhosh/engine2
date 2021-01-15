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
		void addBuffers(std::vector<unsigned>& buffers, const GLenum draw_type = GL_TRIANGLES);
		inline void setMaterial(const std::vector<Materials::Material*>& indices) { materials = std::vector<Materials::Material*>(indices.begin(), indices.end()); };
		inline Component::Type getType() const {
			return Component::Type::RenderMesh;
		}
		void cleanUp();
	private:
		// index of buffer in the resorce loader
		std::vector<Materials::Material*> materials;
		std::vector<unsigned> buffers;
	};
}

