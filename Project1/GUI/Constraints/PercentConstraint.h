#pragma once
#include "ConstraintBase.h"

namespace GUI{
	class PercentConstraint : public ConstraintBase {
	private:
		float ratio;
	public:
		PercentConstraint();
		PercentConstraint(Float ratio);
		~PercentConstraint() = default;
		void cleanUp();
		void solve(Float size, float& axis);
	};
};