#pragma once
#include "../UIElementBase.h"

namespace GUI {
	class GUITextBlock : public GUIElementBase
	{
	private:
		std::string text;
	public:
		GUITextBlock();
		GUITextBlock(String text);
		~GUITextBlock() = default;

		void cleanUp();
		void render();

		// setters
		void setText(String text);

		// getters
		String getText() const;
	};
}

