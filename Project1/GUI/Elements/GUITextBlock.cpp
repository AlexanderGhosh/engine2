#include "GUITextBlock.h"

GUI::GUITextBlock::GUITextBlock() : GUIElementBase(), text("")
{
}

GUI::GUITextBlock::GUITextBlock(String text) : GUITextBlock()
{
	this->text = text;
}

void GUI::GUITextBlock::cleanUp()
{
	GUIElementBase::cleanUp();
}

void GUI::GUITextBlock::render()
{
	GUIElementBase::render();
}

void GUI::GUITextBlock::setText(String text)
{
	this->text = text;
}

String GUI::GUITextBlock::getText() const
{
	return text;
}
