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
		std::string activeAnimation;
		int currentClosestFrame;
	public:
		Animated();
		void startAnimation(const std::string& name);
		Render::Animation::KeyFrame nextFrame();
		void setSkeleton(const Render::Animation::Skeleton& skeleton);
		void addAnimation(const Render::Animation::Animation& animation);
	};
}
