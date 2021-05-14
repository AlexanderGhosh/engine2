#pragma once
#include <unordered_map>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "../../Utils/General.h"

namespace Materials {
	template<class T>
	class MatItemBase;
}
namespace GUI {
	class Font;
	class TextRendering
	{
	private:
		static unsigned shaderId, VAO, VBO, EBO;
		static std::unordered_map<std::string, Font*> loadedFonts;
		static bool isInitalized, isCleandUp;
		static glm::mat4 getCharacterModel(Float x, Float y, Float scale);
	public:
		static void init();
		static void addFont(Font* font);
		static void drawText(String text, float x, Float y, Float scale, Materials::MatItemBase<glm::vec4>* colour, String fontName = "arial", glm::mat4 t = glm::mat4(1));
		static void drawText(String text, float x, Float y, Float scale, Materials::MatItemBase<glm::vec4>* colour, const Font& font, glm::mat4 t);
		static Vector2 getStringDimentions(String text, Unsigned scale, const Font& font);
		static void setShader(Unsigned shaderId);
		static Font* getFont(String name = "arial");
		static void cleanUp();
	};
}

