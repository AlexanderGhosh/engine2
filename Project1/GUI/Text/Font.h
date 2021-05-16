#pragma once
#include "../../Utils/General.h"
#include <unordered_map>

namespace GUI {
	class Character {
	public: 
		unsigned texId;
		glm::vec2 size;
		glm::vec2 bearing;
		unsigned advance;
		Character();
		~Character() = default;
	};
	class Font {
	private:
		std::string name;
		std::unordered_map<char, Character> chars;
	public:
		Font();
		Font(String fontName);
		~Font() = default;
		void cleanUp();
		const Character& getCharacter(const char& character) const;

		String getName() const;
	};
}
