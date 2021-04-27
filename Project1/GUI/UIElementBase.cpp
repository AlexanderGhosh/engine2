#include "UIElementBase.h"
#include "GUIConstraint.h"
#include "ElementContainers/GUIContainerBase.h"

GUI::GUIElementBase::GUIElementBase() : position(0), dimentions(0), constraints(nullptr), baseShape(), parent(nullptr)
{
}

void GUI::GUIElementBase::cleanUp()
{
	baseShape.cleanUp();
}

void GUI::GUIElementBase::render()
{
	baseShape.render(getModel(), dimentions);
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