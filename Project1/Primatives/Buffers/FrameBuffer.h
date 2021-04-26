#pragma once
#include <GL/glew.h>
#include <unordered_map>
#include "../../Utils/General.h"

namespace Primative {
	namespace Buffers {
		class FrameBuffer {
		private:
			std::unordered_map<std::string, unsigned> textures;
			unsigned depthStencilRBO;
			glm::vec3 backgroundColour;
			glm::ivec2 dimentions;
			GLbitfield clearMask;
			unsigned fbo;

			void initalize(long texture_parma);
			std::array<int, 4> getBufferData(String type, unsigned& colAttach) const;
		public:
			FrameBuffer();
			FrameBuffer(const std::vector<std::string>& textures, const glm::ivec2& dimentions, long texture_param = GL_REPEAT, const glm::vec3& bgColour = Utils::zero());
			~FrameBuffer() = default;
			void cleanUp();

			void bind(const long& target = GL_FRAMEBUFFER) const;
			void unBind(const long& target = GL_FRAMEBUFFER) const;
			void clearBits() const;
			/// <summary>
			/// will active the colour unit textures and add to the unit
			/// </summary>
			/// <param name="unit"></param>
			void activateColourTextures(int& unit, const std::vector<std::string>& names);
			Unsigned getTexture(String name);
			Vector2 getDimentions() const;
		};
	}
}