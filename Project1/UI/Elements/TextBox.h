#pragma once
#include "Element.h"
#include <string>
#include "../TextRenderer.h"
namespace UI {
	class TextBox : public Element
	{
	private:
		std::string text, font;
		TextRenderer* textRenderer;
	public:
		TextBox();
		inline const std::string& getText() const { return text; };
		inline void setText(const std::string& text) { this->text = text; };

		inline const std::string& getFont() const { return font; };
		inline void setFont(const std::string& font) 
		{ 
			this->font = font; 
			textRenderer = TextRenderer::getFont(font);
		};

		void drawContent() const;
	};
};

