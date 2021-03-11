#pragma once
#include "Element.h"
#include <unordered_set>
namespace UI {
	class ListView : public Element
	{
	private:
		std::unordered_set<Element*> elements;
		bool horizontal;
		int seperation;
	public:
		ListView();
		ListView(const std::unordered_set<Element*>& elements);
		void addElement(Element* element);
		void removeElement(Element* element);
		inline void makeVertical() { horizontal = true; };
		inline void makeHorizontal() { horizontal = false; };
		inline void setSeperation(const unsigned& sep) { seperation = sep; };
		inline const unsigned& getSeperation() const { return seperation; };
		void checkEvents();
		void drawContent() const; 
		void cleanUp();
	};
};

