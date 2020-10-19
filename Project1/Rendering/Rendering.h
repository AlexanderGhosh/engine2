#pragma once
#include <vector>
#include "../Primatives/Buffers.h"
#include "../Primatives/Mesh.h"
#include "Animation/Animation.h"
using namespace std;
namespace Render {
	class Rendering : public Animation::Animation // special case of animation with only one frame
	{
	public:
		Rendering();
		virtual void draw(unsigned shaderId);
		const void addMesh(Primative::Mesh* m);
	protected:
		vector<Primative::Buffers> buffers;
	};
}

