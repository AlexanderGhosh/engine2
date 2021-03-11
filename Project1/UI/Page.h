#pragma once
#include "Panes/Canvas.h"

namespace UI {
	class Page
	{
	protected:
		Page() : root(nullptr) { };
		Canvas* root;
	public:
		inline const Canvas* getRoot() const { return root; };
		inline void update(float deltaTime)
		{ 
			root->update(deltaTime);
		};
	};
};

