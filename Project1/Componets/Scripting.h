#pragma once
#include "ComponetBase.h"

namespace Component {
	class Scripting : public ComponetBase
	{	
	public:
		Scripting();
		~Scripting() = default;
		virtual void cleanUp();
		inline Type getType() const { return Type::Script; };
	};
}


