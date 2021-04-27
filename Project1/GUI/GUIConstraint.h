#pragma once
#include "../Utils/General.h"

namespace GUI {
	class ConstraintBase;

	class GUIConstraint {
	private:
		ConstraintBase* x, * y, * w, * h;
		static glm::vec2 screenDimentions;
	public:
		GUIConstraint();
		~GUIConstraint() = default;
		void cleanUp();

		void solve(glm::vec2& pos, glm::vec2& dim);

		// setters
		void setX(ConstraintBase* constraint);
		void setY(ConstraintBase* constraint);
		void setWidth(ConstraintBase* constraint);
		void setHeight(ConstraintBase* constraint);
		static void setScreenDimentions(Vector2 dim);

		// getters
		static Vector2 getScreenDimentions();
	};
};

