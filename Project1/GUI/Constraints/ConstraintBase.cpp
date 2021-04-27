#include "ConstraintBase.h"

GUI::ConstraintBase::ConstraintBase() : parent(nullptr)
{
}

void GUI::ConstraintBase::setParent(GUIElementBase* parent)
{
	this->parent = parent;
}
