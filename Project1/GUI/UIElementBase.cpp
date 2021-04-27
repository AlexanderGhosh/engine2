#include "UIElementBase.h"
#include "GUIConstraint.h"
#include "ElementContainers/GUIContainerBase.h"
#include "../EventSystem/Handler.h"

GUI::GUIElementBase::GUIElementBase() : position(0), dimentions(0), constraints(nullptr), baseShape(), parent(nullptr), 
										mouseEnter(nullptr), mouseLeave(nullptr), mouseHover(nullptr), mouseDown(nullptr),
										mouseUp(nullptr), keyDown(nullptr), keyUp(nullptr), cursorDown(false), cursorHover(false), cursorOver(false), keyDown_(false)
{
	auto event = [](GUIElementBase* sender) {};
	mouseEnter = event;
	mouseLeave = event;
	mouseHover = event;

	mouseDown = event;
	mouseUp = event;

	keyDown = event;
	keyUp = event;
}

void GUI::GUIElementBase::cleanUp()
{
	baseShape.cleanUp();
}

void GUI::GUIElementBase::render()
{
	baseShape.render(getModel(), dimentions);
}

void GUI::GUIElementBase::handelEvents()
{
	glm::vec2 cursorPos = Events::Handler::getCursorPos();

	const std::array<glm::vec2, 2> bounds{
		glm::vec2(0), dimentions
	};

	if (Utils::contained(bounds, cursorPos)) {
		if (cursorOver) {
			// cursor hover
			mouseHover(this);
		}
		else {
			// cursor enter
			mouseEnter(this);
		}
		cursorOver = true;
	}
	else if (cursorOver) {
		cursorOver = false;
		mouseLeave(this);
		// cursor leave
	}

	if (cursorOver) { // should do for all mouse buttons
		if (Events::Handler::getCursor(Events::Cursor::Left, Events::Action::Down) AND NOT cursorDown) {
			// mouse down
			cursorDown = true;
			mouseDown(this);
		}
		if (Events::Handler::getCursor(Events::Cursor::Left, Events::Action::Up) AND cursorDown) {
			// mouse up
			cursorDown = false;
			mouseUp(this);
		}
	}
	else {
		cursorDown = false;
	}

	if (Events::Handler::keyDown) {
		keyDown_ = true;
		keyDown(this);
	}
	if (Events::Handler::keyDown AND keyDown_) {
		keyUp(this);
		keyDown_ = false;
	}
}

void GUI::GUIElementBase::setParent(GUIContainerBase* parent)
{
	this->parent = parent;
	constraints->solve(position, dimentions);
}

void GUI::GUIElementBase::setConstraints(GUIConstraint* constraints)
{
	constraints->setParent(this);
	this->constraints = constraints;
}

void GUI::GUIElementBase::setAlbedo(Materials::MatItemBase<glm::vec4>* albedo)
{
	baseShape.setAlbedo(albedo);
}

GUI::GUIContainerBase* GUI::GUIElementBase::getParent() const
{
	return parent;
}

GUI::GUIConstraint* GUI::GUIElementBase::getConstraints() const
{
	return constraints;
}

const glm::mat4 GUI::GUIElementBase::getModel() const
{
	glm::mat4 res(1);
	glm::vec2 pos = position + (parent ? parent->getPosition() : glm::vec2(0));
	pos.y = GUIConstraint::getScreenDimentions().y - (pos.y + dimentions.y);
	res = glm::translate(res, glm::vec3(pos, 0));
	res = glm::scale(res, glm::vec3(dimentions, 0));
	return res;
}