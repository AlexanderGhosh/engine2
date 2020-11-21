#include "Grid.h"
#include "../../Utils/General.h"

UI::Grid::Grid(unsigned width, unsigned height) : Pane(width, height), columns(1), rows(1), elementPositions()
{
}

void UI::Grid::setColumn(Element* element, const unsigned& column)
{
	unsigned i = Utils::findIndex(elements, element);
	elementPositions[i].first = column;
}

void UI::Grid::setRow(Element* element, const unsigned& row)
{
	unsigned i = Utils::findIndex(elements, element);
	elementPositions[i].second = row;
}

void UI::Grid::update()
{
	unsigned columnSize = width / columns;
	unsigned rowSize = height / rows;
	for (unsigned i = 0; i < elementPositions.size(); i++) {
		unsigned c = elementPositions[i].first;
		unsigned r = elementPositions[i].second;
		if (c < 0 || r < 0)
			continue;
		elements[i]->setPos({ c * columnSize, r * rowSize });
	}
	Pane::update();
}
