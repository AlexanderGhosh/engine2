#include "TextBlock.h"

UI::TextBlock::TextBlock() : Element(), text(""), font("arial"), textRenderer(nullptr), forgroundColor()
{
	textRenderer = UI::TextRenderer::getFont(font);
}

void UI::TextBlock::drawContent() const
{
	if (NOT textRenderer)
		return;
	const glm::vec2 size = textRenderer->getStringDimentions(text) / 2.0f;
	const glm::vec2& pos = this->getPos();
	textRenderer->drawText(text, pos.x - size.x, pos.y - size.y, 1, this->getForgroundColor());
}
