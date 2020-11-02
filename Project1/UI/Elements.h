#pragma once
#include <glm.hpp>
#include <string>
#include <array>
#include <functional>
namespace UI {
	class Element
	{
	private:
		glm::vec2 screenPos; // center of the object
		float width, height;
		std::string name;
		Element* content;
		bool cursorOver;
	public:
		Element();
		inline const glm::vec2& getPos() const { return screenPos; };
		inline void setPos(const glm::vec2& pos) { screenPos = pos; };

		inline const float& getWidth() const { return width; };
		inline void setWidth(const float& width) { this->width = width; };

		inline const float& getHeight() const { return height; };
		inline void setHeight(const float& height) { this->height = height; };

		std::array<glm::vec2, 2> getCorners() const;
		void checkEvents();

		void (*mouseEnter)(const Element*);
		void (*mouseLeave)(const Element*);
		void (*mouseHover)(const Element*);
			  
		void (*mouseDown)(const Element*);
		void (*mouseUp)(const Element*);

		glm::mat4 getModel() const;

	};
};

