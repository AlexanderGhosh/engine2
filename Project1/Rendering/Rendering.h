#pragma once
#include <vector>
#include "../Primatives/Buffers.h"
#include "../Utils/ResourceLoader.h"
#include "../Primatives/Mesh.h"
#include "Animation/Animation.h"
#include "../Componets/Componets.h"
namespace Render {
	class RenderMesh : public Animation::Animation, public Componet::Base // special case of animation with only one frame
	{
	public:
		RenderMesh();
		virtual void update();
		void addMesh(Primative::Mesh* m);
		inline void setShader(std::string name) { 
			shaderName = name;
			shaderId = ResourceLoader::getShader(shaderName);
		};

	private:
		std::vector<Primative::Buffers> buffers;
		std::string shaderName;
		GameObject* parent;
		unsigned shaderId;
	};
}

