#include "ListBox.h"

UI::ListView::ListView() : Element(), horizontal(1), elements(), seperation(0)
{
}

UI::ListView::ListView(const std::unordered_set<Element*>& elements) : ListView()
{
	this->elements = elements;
}

void UI::ListView::addElement(Element* element)
{
	elements.insert(element);
}

void UI::ListView::removeElement(Element* element)
{
	elements.erase(element);
}

void UI::ListView::checkEvents()
{
	char index = horizontal ? 0 : 1;
	glm::vec2 p(0);
	for (Element* item : elements) {
		item->setPos(p);
		if (horizontal) {
			p.x += item->getWidth() + seperation;
		}
		else{
			p.y += item->getHeight() + seperation;
		}
		item->checkEvents();
	}
	Element::checkEvents();
}

void UI::ListView::drawContent() const
{
	for (const Element* item : elements) {
		item->drawContent();
	}
}

void UI::ListView::cleanUp()
{
	for (auto itt = elements.begin(); itt != elements.end();) {
		(*itt)->cleanUp();
		itt = elements.erase(itt);
	}
}
