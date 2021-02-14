#pragma once
#include <unordered_map>
#include "../Rendering/Animation/Bones.h"
#include "../Rendering/Animation/Animation.h"
#include "Componets.h"

namespace Component {
	class Animated : public ComponetBase
	{
	private:
		std::unordered_map<std::string, Render::Animation::Animation> animations;
		Render::Animation::Skeleton skeleton;
	public:
		Animated();
		void startAnimation(const std::string& name) const;
	};
}
