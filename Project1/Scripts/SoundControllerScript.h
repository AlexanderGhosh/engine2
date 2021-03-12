#pragma once
#include "../Componets/Scripting.h"
class SoundControllerScript : public Component::Scripting
{
private:
public:
	SoundControllerScript();
	~SoundControllerScript() = default;
	void keyButton(float deltaTime);
};

