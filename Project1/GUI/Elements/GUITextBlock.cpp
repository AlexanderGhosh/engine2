#include "GUITextBlock.h"
#include "../Text/TextRendering.h"
#include "../../Primatives/Materials/MatItemBase.h"

GUI::GUITextBlock::GUITextBlock() : GUIElementBase(), text(""), textColour(nullptr), fontSize(12)
{
}

GUI::GUITextBlock::GUITextBlock(String text, Int fontSize) : GUITextBlock()
{
	this->text = text;
	this->fontSize = fontSize;
}

void GUI::GUITextBlock::cleanUp()
{
	GUIElementBase::cleanUp();
	TextRendering::cleanUp();
}

void GUI::GUITextBlock::render()
{
	TextRendering::drawText(text, position.x, position.y, fontSize, textColour, "arial", getModel());
	GUIElementBase::render();
}

void GUI::GUITextBlock::setText(String text)
{
	this->text = text;
}

void GUI::GUITextBlock::setTextColour(Materials::MatItemBase<glm::vec4>* colour)
{
	textColour = colour;
}

String GUI::GUITextBlock::getText() const
{
	return text;
}
