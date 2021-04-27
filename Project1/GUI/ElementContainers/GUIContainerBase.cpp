#include "GUIContainerBase.h"
#include "../UIElementBase.h"
#include "../../Rendering/Shading/Manager.h"
#include "../GUIConstraint.h"

GUI::GUIContainerBase::GUIContainerBase() : children(), baseShape(), position(0), dimentions(1)
{
}

glm::mat4 GUI::GUIContainerBase::getModel() const
{
	glm::mat4 res(1);
	glm::vec2 pos = position;
	pos.y = GUIConstraint::getScreenDimentions().y - (pos.y + dimentions.y);
	res = glm::translate(res, glm::vec3(pos, 0));
	res = glm::scale(res, glm::vec3(dimentions, 0));
	return res;
}

void GUI::GUIContainerBase::cleanUp()
{
	for (auto itt = children.begin(); itt != children.end();) {
		(*itt)->cleanUp();
		itt = children.erase(itt);
	}
	baseShape.cleanUp();
}

void GUI::GUIContainerBase::render()
{
	for (auto child : children) {
		child->render();
	}
	baseShape.render(getModel(), dimentions);
}

void GUI::GUIContainerBase::handleEvents()
{
	for (auto child : children) {
		child->handelEvents();
	}
}

void GUI::GUIContainerBase::setPosition(Vector2 pos)
{
	position = pos;
}

void GUI::GUIContainerBase::setDimentions(Vector2 dim)
{
	dimentions = dim;
}

void GUI::GUIContainerBase::setBaseAlbedo(Materials::MatItemBase<glm::vec4>* albedo)
{
	baseShape.setAlbedo(albedo);
}

void GUI::GUIContainerBase::setCornerRadius(Float r)
{
	baseShape.setRadius(r);
}

void GUI::GUIContainerBase::addElement(GUIElementBase* element)
{
	element->setParent(this);
	children.push_back(element);
}

Vector2 GUI::GUIContainerBase::getPosition() const
{
	return position;
}

Vector2 GUI::GUIContainerBase::getDimentions() const
{
	return dimentions;
}
