#pragma once
#include <unordered_map>
#include "../Rendering/Animation/Bones.h"
#include "Componets.h"

namespace Primative {
	namespace Buffers {
		class TextureBuffer;
	}
}
namespace Render {
	namespace Animation {
		class Animation;
		struct KeyFrame;
	}
}

namespace Component {
	class Animated : public ComponetBase
	{
	private:
		std::unordered_map<std::string, Render::Animation::Animation*> animations;
		std::string activeAnimation;
		float runTime;
		bool running;
		static Primative::Buffers::TextureBuffer bonesBuffer;
		static bool initalized;
		bool isActive();
	public:
		Animated();
		~Animated() = default;
		void startAnimation(String name, bool start = true);
		void stopAnimation();
		void playAnimation();
		void togglePlay();
		const Render::Animation::KeyFrame& nextFrame();
		const Render::Animation::KeyFrame getCurrentFrame();
		void addAnimation(Render::Animation::Animation* animation);
		void cleanUp();
		inline Component::Type getType() const {
			return Component::Type::Animated;
		}
		void update(float deltaTime);
		const Render::Animation::Animation* getActiveAnimation();
		void bind();
		void unbind();
	};
}
