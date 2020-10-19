#pragma once
namespace Render {
	namespace Animation {
		class Animation
		{
		private:
			unsigned frameCount;
			unsigned duration;
			int currentFrame;
		public:
			inline Animation() : frameCount(1), duration(0), currentFrame(-1) { };
			inline Animation(unsigned frameCnt, unsigned dur) : frameCount(frameCnt), duration(dur), currentFrame(-1) { }
			const inline unsigned getFrameCount() { return frameCount; }
			const inline unsigned getDuration() { return duration; };
			const void* getNextFrame();
		};
	}
}

