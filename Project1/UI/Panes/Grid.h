#pragma once
#include "Pane.h"
namespace UI {
	class Grid : public Pane
	{
	private:
		unsigned columns, rows;
		std::vector<std::pair<unsigned, unsigned>> elementPositions;
	public:
		Grid(unsigned width, unsigned height);
		inline void addElement(Element* element) 
		{ 
			elements.push_back(element); 
			elementPositions.push_back({ 0, 0 });
		};
		inline void setColumnCount(const unsigned& columns) { this->columns = columns; };
		inline void setRowCount(const unsigned& rows) { this->rows = rows; };
		void setColumn(Element* element, const unsigned& column);
		void setRow(Element* element, const unsigned& row);
		void update();
	};
};

