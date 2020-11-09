#pragma once
#include "Panes/Pane.h"
namespace UI {
	class Page
	{
	private:
		Page() : root(nullptr) { };
		Pane* root;
	public:
		virtual void init() = 0;
	};
};

