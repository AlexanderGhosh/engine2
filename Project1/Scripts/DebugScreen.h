#pragma once
#include "../Componets/Scripting.h"
#include "../UI/Elements/TextBlock.h"
#include "../UI/Panes/Canvas.h"

class DebugScreen : public Component::Scripting
{
public:
	DebugScreen(Vector3 colour = glm::vec3(0));
	~DebugScreen() = default;
	void awake();
	void update(float deltaTime);
	void cleanUp();
private:
	glm::ivec2 screenDim;
	glm::vec3 bgColour;
	float counter;
	UI::Canvas mainCanvas;
	UI::TextBlock fpsText, position;
};

