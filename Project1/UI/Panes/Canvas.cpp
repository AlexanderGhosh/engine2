#include "Canvas.h"
#include "../UIRenderer.h"

UI::Canvas::Canvas(unsigned width, unsigned height) : elements(), width(width), height(height)
{

};

void UI::Canvas::addElement(Element* element)
{
	elements.push_back(element);
}

void UI::Canvas::addElement(Element& element)
{
	elements.push_back(&element);
}

void UI::Canvas::update(float deltaTime)
{
	for (Element* element : elements) {
		element->checkEvents();
		UI::UIRenderer::render(element);
	}
}

const std::vector<UI::Element*>& UI::Canvas::getElements() const {
	return elements;
}

const glm::ivec2 UI::Canvas::getDimentions() const
{
	return { width, height };
};

void UI::Canvas::cleanUp()
{
	for (auto itt = elements.begin(); itt != elements.end();) {
		(*itt)->cleanUp();
		itt = elements.erase(itt);
	}
}
