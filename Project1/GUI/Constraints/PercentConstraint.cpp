#include "PercentConstraint.h"
#include "../UIElementBase.h"
#include "../ElementContainers/GUIContainerBase.h"

GUI::PercentConstraint::PercentConstraint() : ConstraintBase(), ratio(0)
{
}

GUI::PercentConstraint::PercentConstraint(Float ratio) : PercentConstraint()
{
	this->ratio = ratio;
}

void GUI::PercentConstraint::cleanUp()
{
}

void GUI::PercentConstraint::solve(Float size, float& axis)
{
	axis = size * ratio;
}
