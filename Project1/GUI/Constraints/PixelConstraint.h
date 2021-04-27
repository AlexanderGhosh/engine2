#pragma once
#include "ConstraintBase.h"

namespace GUI {
	class PixelConstraint : public ConstraintBase
	{
	private:
		float pixels;
	public:
		PixelConstraint();
		PixelConstraint(Float pixelValue);
		~PixelConstraint() = default;
		void cleanUp();
		void solve(float& axis);
	};
};

