#pragma once
#include <string>
#include <vector>
#include "../Rendering/Animation/Bones.h"
namespace Primative {
	class Model
	{
	private:
		std::string name;
		std::vector<unsigned> buffers;
		Render::Animation::Skeleton skeleton;
	public:
		Model();
		const std::string& getName() const;
		void setName(const std::string& name);

		const Render::Animation::Skeleton& getSkeleton() const;
		void setSkeleton(const Render::Animation::Skeleton& skeleton);
	};
}

