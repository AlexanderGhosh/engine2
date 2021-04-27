#pragma once
#include "ConstraintBase.h"

namespace GUI {
	class SpanConstraint : public ConstraintBase
	{
	public:
		SpanConstraint();
		~SpanConstraint() = default;
		void cleanUp();
		void solve(Float size, float& axis);
	};
}

