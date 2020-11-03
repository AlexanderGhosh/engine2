#pragma once
#include <vector>
#include "../Elements/Element.h"
namespace UI {
	class Pane
	{
	private:
		std::vector<Element*> elements;
	public:
		inline Pane() : elements() { };
		inline void addElement(Element* element) { elements.push_back(element); };
		void update();
		inline const std::vector<Element*>& getElements() const { return elements; };
	};
};

