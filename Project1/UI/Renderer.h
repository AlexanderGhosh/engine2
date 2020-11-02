#pragma once
#include "../Primatives/Buffers.h"
#include "Pane.h"
namespace UI {
	class Renderer
	{
		static glm::mat4 projectionMatrix;
		static unsigned shaderId;
		static Primative::Buffers quadBuffer;
		static Primative::StaticBuffer uiBuffer;
	public:
		static void init(const unsigned& shaderId, const glm::vec2& screenDim);
		static void render(const Pane* pane);
	};
};

