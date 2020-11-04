#pragma once
#include "TextBlock.h"
namespace UI {
	class Button : public TextBlock
	{
	private:
		const short clickTimeOut = 20; // in frames
		short clickTimer;
		bool clicking;
	public:
		Button();
		virtual void checkEvents();
		void (*click)(Element*);
	};
};

