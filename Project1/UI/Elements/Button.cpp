#include "Button.h"

UI::Button::Button() : clickTimer(0), clicking(0) {
	click = [](const Element* sender) {

	};
}

void UI::Button::checkEvents()
{
	TextBlock::Element::checkEvents();
	if (clicking)
		this->clickTimer++;
	if (this->mDown) {
		clicking = true;
		this->clickTimer = 0;
	}
	else if(clickTimer >= clickTimeOut || !this->mDown && clicking) {
		clicking = false;
		if (!this->mDown && this->cursorOver) {
			click(this);
		}
	}
}
