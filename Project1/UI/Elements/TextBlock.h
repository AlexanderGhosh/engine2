#pragma once
#include "Element.h"
#include <string>
#include "../TextRenderer.h"
namespace UI {
	class TextBlock : public Element
	{
	private:
		std::string text, font;
		TextRenderer* textRenderer;
		glm::vec3 forgroundColor;
	public:
		TextBlock();
		inline const std::string& getText() const { return text; };
		inline void setText(const std::string& text) { this->text = text; };

		inline const std::string& getFont() const { return font; };
		inline void setFont(const std::string& font) 
		{ 
			this->font = font; 
			textRenderer = TextRenderer::getFont(font);
		};

		inline const glm::vec3& getForgroundColor() const { return forgroundColor; };
		inline void setForgroundColor(const glm::vec3& col) { forgroundColor = col; };

		void drawContent() const;
	};
};

