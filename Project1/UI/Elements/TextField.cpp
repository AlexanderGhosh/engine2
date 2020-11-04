#include "TextField.h"
#include "../../EventSystem/Handler.h"
#include <algorithm>

UI::TextField::TextField() : Button(), flashCounter(0), flashed(1), selected(0)
{
	this->click = [](Element* sender) {
		TextField* t = dynamic_cast<TextField*>(sender);
		t->toggleSelected();
		Events::Handler::lastChar = "";
	};
}

void UI::TextField::checkEvents()
{
	if (selected) {
		setText(getText() + Events::Handler::lastChar);
		Events::Handler::lastChar = "";
	}

	flashCounter++;
	flashCounter %= flashInterval;

	if (flashCounter == 0) {
		flashed ^= 1;
	}
	/*if (flashed) {
		Button::TextBlock::setText(getText() + " |");
		flashed = false;
	}
	else {
		std::string str = getText();
		str.pop_back();
		str.pop_back();
		// str.erase(std::remove(str.begin(), str.end(), '|'), str.end());
		setText(str);
	}*/
	Button::checkEvents();
}