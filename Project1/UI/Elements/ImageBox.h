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
			this->backgroundColor(backgroundColor);
		};
		void drawContent() const;
	};
};

