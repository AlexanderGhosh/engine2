#pragma once
#include "Panes/Pane.h"
#include "Page.h"

namespace  Primative {
	namespace Buffers {
		class VertexBuffer;
		class StaticBuffer;
	}
}
namespace UI {
	class UIRenderer
	{
		static unsigned shaderId;
		static Primative::Buffers::VertexBuffer quadBuffer;
		static Primative::Buffers::StaticBuffer uiBuffer;
	public:
		static void init(const unsigned& shaderId, const glm::vec2& screenDim);
		static void render(const Pane* pane);
		static void render(const Page* pane);
		static void render(const Element* element);
		static void cleanUp();
	};
};

