#pragma once
#include "ComponetBase.h"
namespace Component {
	class AudioReciever : public ComponetBase
	{
	private:
		char placeHolder;
	public:
		AudioReciever();
		~AudioReciever() = default;
		void update(float deltaTime);
		void cleanUp();
		inline Type getType() const { return Type::AudioReciever; };
	};
}

