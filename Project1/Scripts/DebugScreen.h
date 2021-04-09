#pragma once
#include "../Componets/Scripting.h"
#include "../UI/Elements/TextBlock.h"
#include "../UI/Panes/Canvas.h"

class DebugScreen : public Component::Scripting
{
public:
	DebugScreen();
	~DebugScreen() = default;
	void awake();
	void update(float deltaTime);
	void cleanUp();
	GameObject* RedBlock, *BlueBlock;
private:
	glm::ivec2 screenDim;
	float counter;
	UI::Canvas mainCanvas;
	UI::TextBlock fpsText, position, redBlockPosition, redBlockVelocity, 
		divider,
		blueBlockPosition, blueBlockVelocity,
		blockDistance;
};

