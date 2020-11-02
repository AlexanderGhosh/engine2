#include "Pane.h"

void UI::Pane::update()
{
	for (Element* element : elements) {
		element->checkEvents();
	}
}
