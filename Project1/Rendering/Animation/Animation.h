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
			float timeStamp;
			void cleanUp();
			KeyFrame interpolate(const KeyFrame& frame, Float ratio) const;
		};
		class Animation
		{
		private:
			std::string name;
			float duration, tps;
			float getScaleFactor(Float ticks, Float nextTS, Float lastTS) const;
		public:
			std::vector<KeyFrame> keyFrames;
			Animation();
			Animation(String name, Float duration, Float tps);
			const KeyFrame getFrame(Float timeInSecconds) const;
			const KeyFrame& getFrameABS(Int frame) const;
			Unsigned getFrameCount() const;

			void addKeyFrame(const KeyFrame& frame);
			String getName() const;
			Float getTPS() const;

			Float getDuration() const;

			void cleanUp();
		};
	}
}

