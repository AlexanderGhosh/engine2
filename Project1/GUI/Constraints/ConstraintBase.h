#pragma once
#include "../../Utils/General.h"

namespace GUI {
	class ConstraintBase
	{
	private:
	public:
		ConstraintBase();
		~ConstraintBase() = default;
		virtual void cleanUp() = 0;
		virtual void solve(float& axis) = 0;
	};
};

