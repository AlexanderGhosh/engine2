#include "Elements.h"
#include "../EventSystem/Handler.h"
#include "../Utils/General.h"

UI::Element::Element() : screenPos(0), width(0), height(0), name(""), content(nullptr), cursorOver(0)
{
	auto event = [](const Element* sender) { };
	mouseEnter = event;
	mouseLeave = event;
	mouseHover = event;

	mouseDown = event;
	mouseUp = event;
}

std::array<glm::vec2, 2> UI::Element::getCorners() const
{
	glm::vec2 min = screenPos;
	min.x -= width / 2.0f;
	min.y -= height / 2.0f;
	glm::vec2 max = min + glm::vec2(width, height);
	return { min, max };
}

void UI::Element::checkEvents()
{
	// check events

	glm::vec2 cursorPos = Events::Handler::getCursorPos();

	const auto bounds = this->getCorners();

	if (Utils::contained(bounds, cursorPos)) {
		if (this->cursorOver) {
			// cursor hover
			this->mouseHover(this);
		}
		else {
			// cursor enter
			this->mouseEnter(this);
		}
		this->cursorOver = true;
	}
	else if (this->cursorOver) {
		this->cursorOver = false;
		// cursor leave
		this->mouseLeave(this);
	}

	if (this->cursorOver) { // should do for all mouse buttons
		if (Events::Handler::getCursor(Events::Cursor::Any, Events::Action::Down)) {
			// mouse down
			this->mouseDown(this);
		}
		if (Events::Handler::getCursor(Events::Cursor::Any, Events::Action::Up)) {
			// mouse up
			this->mouseUp(this);
		}
	}
}

glm::mat4 UI::Element::getModel() const
{
	glm::mat4 model(1);
	model = glm::translate(model, glm::vec3(screenPos, 0));
	model = glm::scale(model, glm::vec3(width, height, 0)/2.0f);
	return model;
}
