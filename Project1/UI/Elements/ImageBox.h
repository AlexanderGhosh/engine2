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
			this->backgroundColor.setValue(backgroundColor);
		};
		void drawContent() const;
	};
};

