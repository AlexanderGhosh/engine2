#include "GUIConstraint.h"
#include "Constraints/ConstraintBase.h"

glm::vec2 GUI::GUIConstraint::screenDimentions(0);
GUI::GUIConstraint::GUIConstraint() : x(nullptr), y(nullptr), h(nullptr), w(nullptr)
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

Vector2 GUI::GUIConstraint::getScreenDimentions()
{
	return screenDimentions;
}

void GUI::GUIConstraint::setScreenDimentions(Vector2 dim)
{
	screenDimentions = dim;
}

void GUI::GUIConstraint::solve(glm::vec2& pos, glm::vec2& dim)
{
	x->solve(pos.x);
	y->solve(pos.y);
	w->solve(dim.x);
	h->solve(dim.y);
}

void GUI::GUIConstraint::cleanUp()
{
	x->cleanUp();
	y->cleanUp();
	w->cleanUp();
	h->cleanUp();
}
