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
		std::string activeAnimation;
		float runTime;
	public:
		Animated();
		~Animated() = default;
		void startAnimation(const std::string& name);
		const Render::Animation::KeyFrame& nextFrame();
		const Render::Animation::KeyFrame getCurrentFrame();
		void addAnimation(const Render::Animation::Animation& animation);
		void cleanUp();
		inline Component::Type getType() const {
			return Component::Type::Animated;
		}
		void update(float deltaTime);
	};
}
