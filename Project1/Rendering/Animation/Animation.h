#pragma once
#include <vector>
#include <glm.hpp>
#include "../../Utils/General.h"
namespace Render {
	namespace Animation {
		struct KeyFrame {
			/// <summary>
			/// shares the index with the skeletons bones
			/// </summary>
			std::vector<glm::mat4> translations;
			void cleanUp();
		};
		class Animation
		{
		private:
			std::string name;
			float duration;
		public:
			std::vector<KeyFrame> keyFrames;
			Animation();
			Animation(String name, Float duration);
			const KeyFrame& getFrame(int frame) const;
			Unsigned getFrameCount() const;

			void addKeyFrame(const KeyFrame& frame);
			String getName() const;

			void cleanUp();
		};
	}
}

