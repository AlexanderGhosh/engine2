#pragma once
#include <glm.hpp>
#include <string>
#include <array>
#include <functional>
#include "../../Primatives/Material.h"
namespace UI {
	class Element
	{
	protected:
		glm::vec2 screenPos; // center of the object
		float width, height;
		glm::vec4 margin;
		std::string name;
		bool cursorOver, mDown;
		Materials::MatItem backgroundColor;
		Element();
	public:

		inline const glm::vec4& getMargin() const { return margin; };
		inline void setMargin(const glm::vec4& margin) { this->margin = margin; };

		const glm::vec2& getPos() const;
		inline void setPos(const glm::vec2& pos) { screenPos = pos; };

		inline const float getWidth() const { return width; };
		inline void setWidth(const float& width) { this->width = width; };

		virtual inline const float getHeight() const { return height; };
		virtual inline void setHeight(const float& height) { this->height = height; };

		inline const Materials::MatItem& getBackgroundColor() const { return backgroundColor; };
		inline void setBackgroundColor(const glm::vec3& backgroundColor) 
		{ 
			this->backgroundColor(backgroundColor);
		};

		std::array<glm::vec2, 2> getCorners() const;
		virtual void checkEvents();

		void (*mouseEnter)(Element*);
		void (*mouseLeave)(Element*);
		void (*mouseHover)(Element*);
			  
		void (*mouseDown)(Element*);
		void (*mouseUp)(Element*);

		glm::mat4 getModel() const;

		virtual void drawContent() const = 0;

		virtual void cleanUp();
	};
};

