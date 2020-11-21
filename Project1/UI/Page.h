#pragma once
#include "Panes/Pane.h"
namespace UI {
	class Page
	{
	protected:
		Page() : root(nullptr) { };
		Pane* root;
	public:
		inline const Pane* getRoot() const { return root; };
		inline void update() 
		{ 
			root->update(); 
		};
	};
};

