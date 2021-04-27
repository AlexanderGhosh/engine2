#pragma once
#include "../Primatives/Buffers/VertexBuffer.h"

namespace Materials {
	template<class T>
	class MatItemBase;
}
namespace GUI {
	class GUIBaseShape {
	private:
		Materials::MatItemBase<glm::vec4>* albedo;
		float radius;
		static Primative::Buffers::VertexBuffer quadBuffer;
		static unsigned shaderId;
	public:
		GUIBaseShape();
		~GUIBaseShape() = default;
		void cleanUp();
		void render(Matrix4 model, Vector2 dimetionsTranslated);

		// setters
		void setAlbedo(Materials::MatItemBase<glm::vec4>* albedo);
		void setRadius(Float r);
	};
};