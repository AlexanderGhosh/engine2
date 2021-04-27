#pragma once
#include "../../Utils/General.h"

namespace GUI {
	class GUIElementBase;
	class ConstraintBase
	{
	protected:
		GUIElementBase* parent;
	public:
		ConstraintBase();
		~ConstraintBase() = default;
		virtual void cleanUp() = 0;
		virtual void solve(Float size, float& axis) = 0;
		void setParent(GUIElementBase* parent);
	};
};

