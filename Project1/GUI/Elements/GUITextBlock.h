#pragma once
#include "../UIElementBase.h"
namespace Materials {
	template<class T>
	class MatItemBase;
}
namespace GUI {
	class GUITextBlock : public GUIElementBase
	{
	private:
		std::string text;
		int fontSize;
		Materials::MatItemBase<glm::vec4>* textColour;
	public:
		GUITextBlock();
		GUITextBlock(String text, Int fontSize = 1);
		~GUITextBlock() = default;

		void cleanUp();
		void render();

		// setters
		void setText(String text);
		void setTextColour(Materials::MatItemBase<glm::vec4>* colour);

		// getters
		String getText() const;
	};
}

