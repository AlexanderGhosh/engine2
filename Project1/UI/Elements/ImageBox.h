#pragma once
#include <string>
#include "Element.h"
namespace UI {
	class ImageBox : public Element
	{
	public:
		ImageBox();
		inline void setBackgroundImage(const unsigned& backgroundColor)
		{
			this->backgroundColor->addValue(backgroundColor);
		};
		void drawContent() const;
	};
};

