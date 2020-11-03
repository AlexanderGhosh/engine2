#include "TextBox.h"

UI::TextBox::TextBox() : Element(), text(""), font("arial"), textRenderer(nullptr)
{
	textRenderer = UI::TextRenderer::getFont(font);
}

void UI::TextBox::drawContent() const
{
	const glm::vec2 size = textRenderer->getStringDimentions(text) / 2.0f;
	const glm::vec2& pos = this->getPos();
	textRenderer->drawText(text, pos.x - size.x, pos.y - size.y, 1, this->getBackgroundColor());
	// textRenderer->drawText(text, )
}
