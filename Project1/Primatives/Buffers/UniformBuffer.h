#pragma once
#include "../../Utils/General.h"
namespace Primative {
	namespace Buffers {
		class StaticBuffer {
		private:
			unsigned UBO;
			int bindingPoint;
			std::vector<std::pair<unsigned, unsigned>> positions;
			// statics
			static int usedBindingPoint;
			static int maxBingingPoints;
			// private functions
			const void init(unsigned dataSize, short bindingPoint = -1);
		public:
			StaticBuffer();
			StaticBuffer(String types, char bindingPoint = -1);
			StaticBuffer(const std::vector<std::string>& types, char bindingPoint = -1);
			~StaticBuffer() = default;

			const short getBindingPoint() const;

			void fill(unsigned offset, unsigned size, const void* data) const;
			void fill(unsigned position, const void* data) const;

			void bind() const;
			void unBind() const;

			void cleanUp();
		};
	}
}

