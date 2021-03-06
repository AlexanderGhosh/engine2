#pragma once
#include <GL/glew.h>
#include "../../Utils/General.h"

namespace Primative {
	namespace Buffers {
		class TextureBuffer {
		private:
			unsigned VBO, TAO, dataSize;
			GLenum typeContained;
		public:
			TextureBuffer();
			TextureBuffer(String typeContained, Unsigned dataSize);
			~TextureBuffer() = default;

			void fill(const void* data, Unsigned dataSize, int offset = 0);

			void bind() const;
			void unBind() const;

			void cleanUp();
		};
	}
}

