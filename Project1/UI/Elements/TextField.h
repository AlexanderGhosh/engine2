#pragma once
#include "Button.h"
namespace UI {
	class TextField : public Button
	{
	private:
		const short flashInterval = 30;
		short flashCounter;
		bool flashed, selected;
	public:
		inline void toggleSelected() { selected ^= 1; };
		TextField();
		void checkEvents();
	};
};

