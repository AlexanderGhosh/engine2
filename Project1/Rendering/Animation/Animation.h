#pragma once
#include <vector>
#include <glm.hpp>
#include "../../Utils/General.h"
struct aiAnimation;
namespace Render {
	namespace Animation {
		struct KeyFrame {
			/// <summary>
			/// shares the index with the skeletons bones
			/// </summary>
			std::vector<glm::mat4> translations;
		};
		class Animation
		{
		private:
			std::string name;
			float duration;
			std::vector<KeyFrame> keyFrames;
		public:
			Animation();
			Animation(String name, Float duration);
			const void* getNextFrame();

			void addKeyFrame(const KeyFrame& frame);
		};
	}
}

