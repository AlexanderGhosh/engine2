#pragma once
#include <vector>
#include "../Elements/Element.h"
namespace UI {
	class Pane
	{
	protected:
		std::vector<Element*> elements;
		int width, height;
	public:
		inline Pane(unsigned width = 0, unsigned height = 0) : elements(), width(width), height(height) { };
		virtual inline void addElement(Element* element) { elements.push_back(element); };
		virtual void update();
		inline const std::vector<Element*>& getElements() const { return elements; };
	};
};

