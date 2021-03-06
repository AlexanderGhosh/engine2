#pragma once
#include <glm.hpp>
#include <unordered_map>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "../Utils/General.h"
namespace UI {
	struct Character {
		unsigned texId;
		glm::ivec2 size;
		glm::ivec2 bearing;
		unsigned advance;
	};
	class TextRenderer
	{
	private:
		static unsigned shaderId;
		static std::unordered_map<std::string, TextRenderer*> loadedFonts;
		std::unordered_map<char, Character> chars;
		unsigned VAO, VBO;
	public:
		TextRenderer(const glm::vec2& screenDim, String fontName = "arial");
		void drawText(String text, float x, float y, float scale, glm::vec3 color) const;
		const glm::vec2& getStringDimentions(String text, Unsigned scale = 1) const;
		inline static void setShader(Unsigned shaderId) { UI::TextRenderer::shaderId = shaderId; };
		static TextRenderer* getFont(String name = "arial");
		static void cleanUpStatic();
		void cleanUp();
	};
};

