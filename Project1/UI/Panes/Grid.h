#pragma once
#include "Canvas.h"
namespace UI {
	class Grid : public Canvas
	{
	private:
		unsigned columns, rows;
		std::vector<std::pair<unsigned, unsigned>> elementPositions;
	public:
		Grid(unsigned width, unsigned height);
		~Grid() = default;
		void addElement(Element* element);
		void addElement(Element& element);
		void setColumnCount(const unsigned& columns);
		void setRowCount(const unsigned& rows);
		void setColumn(Element* element, const unsigned& column);
		void setRow(Element* element, const unsigned& row);
		void update(float deltaTime);
	};
};

