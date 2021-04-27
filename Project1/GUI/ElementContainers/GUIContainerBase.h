#pragma once
#include "../GUIBaseShape.h"
#include "../../GameObject/GameObject.h"

namespace GUI {
	class GUIElementBase;
	class GUIContainerBase
	{
	protected:
		std::vector<GUIElementBase*> children;
		GUIBaseShape baseShape;
		glm::vec2 position, dimentions;
		GUIContainerBase();
	public:
		~GUIContainerBase() = default;
		virtual void cleanUp();
		virtual void render();

		// setters
		void setPosition(Vector2 pos);
		void setDimentions(Vector2 dim);
		virtual void setBaseAlbedo(Materials::MatItemBase<glm::vec4>* albedo);
		virtual void setCornerRadius(Float r);

		// adders
		virtual void addElement(GUIElementBase* element);

		// getters
		Vector2 getPosition() const;
		Vector2 getDimentions() const;
		glm::mat4 getModel() const;
	};
};

