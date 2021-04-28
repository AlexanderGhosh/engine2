#pragma once
#include "GUIBaseShape.h"

namespace GUI {
	class GUIContainerBase;
	class GUIConstraint;
	class GUIElementBase
	{
	protected:
		GUIContainerBase* parent;
		glm::vec2 position, dimentions;
		GUIConstraint* constraints;
		GUIBaseShape baseShape;
		virtual const glm::mat4 getModel() const;
		// Event Data
		bool cursorOver, cursorDown, cursorHover, keyDown_;
	public:
		GUIElementBase();
		~GUIElementBase() = default;
		virtual void cleanUp();
		virtual void render();

		// setters
		virtual void setParent(GUIContainerBase* parent);
		virtual void setConstraints(GUIConstraint* constraints);
		virtual void setAlbedo(Materials::MatItemBase<glm::vec4>* albedo);

		// getters
		GUIContainerBase* getParent() const;
		GUIConstraint* getConstraints() const;

		// events
		virtual void handelEvents();
		void (*mouseHover)(GUIElementBase* sender);
		void (*mouseEnter)(GUIElementBase* sender);
		void (*mouseLeave)(GUIElementBase* sender);
		void (*mouseUp   )(GUIElementBase* sender);
		void (*mouseDown )(GUIElementBase* sender);
		void (*keyDown   )(GUIElementBase* sender);
		void (*keyUp     )(GUIElementBase* sender);
	};
};

