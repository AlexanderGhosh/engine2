#pragma once
#include "../Utils/General.h"

namespace GUI {
	class ConstraintBase;
	class GUIElementBase;
	class GUIConstraint {
	private:
		ConstraintBase* x, * y, * w, * h;
		GUIElementBase* parent;
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
		void setParent(GUIElementBase* parent);
		static void setScreenDimentions(Vector2 dim);

		// getters
		static Vector2 getScreenDimentions();
		GUIElementBase* getParent() const;
	};
};

