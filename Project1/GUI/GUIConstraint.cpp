#include "GUIConstraint.h"
#include "Constraints/ConstraintBase.h"
#include "UIElementBase.h"
#include "ElementContainers/GUIContainerBase.h"

glm::vec2 GUI::GUIConstraint::screenDimentions(0);

GUI::GUIConstraint::GUIConstraint() : x(nullptr), y(nullptr), h(nullptr), w(nullptr), parent(nullptr)
{
}

void GUI::GUIConstraint::setX(ConstraintBase* constraint)
{
	x = constraint;
}

void GUI::GUIConstraint::setY(ConstraintBase* constraint)
{
	y = constraint;
}

void GUI::GUIConstraint::setWidth(ConstraintBase* constraint)
{
	w = constraint;
}

void GUI::GUIConstraint::setHeight(ConstraintBase* constraint)
{
	h = constraint;
}

void GUI::GUIConstraint::setParent(GUIElementBase* parent)
{
	this->parent = parent;
}

void GUI::GUIConstraint::setScreenDimentions(Vector2 dim)
{
	screenDimentions = dim;
}

Vector2 GUI::GUIConstraint::getScreenDimentions()
{
	return screenDimentions;
}

GUI::GUIElementBase* GUI::GUIConstraint::getParent() const
{
	return parent;
}

void GUI::GUIConstraint::solve(glm::vec2& pos, glm::vec2& dim)
{
	Vector2 dimentions = parent->getParent()->getDimentions();
	if(x)
		x->solve(dimentions.x, pos.x);
	if (y)
		y->solve(dimentions.y, pos.y);
	if (w)
		w->solve(dimentions.x, dim.x);
	if (h)
		h->solve(dimentions.y, dim.y);
}

void GUI::GUIConstraint::cleanUp()
{
	if (x)
		x->cleanUp();
	if (y)
		y->cleanUp();
	if (w)
		w->cleanUp();
	if (h)
		h->cleanUp();
}
