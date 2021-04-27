#include "PixelConstraint.h"
#include "../GUIConstraint.h"

GUI::PixelConstraint::PixelConstraint() : ConstraintBase(), pixels(0)
{
}

GUI::PixelConstraint::PixelConstraint(Float pixelValue)
{
	pixels = pixelValue;
}

void GUI::PixelConstraint::cleanUp()
{
}

void GUI::PixelConstraint::solve(Float size, float& axis)
{
	axis = pixels;
}
