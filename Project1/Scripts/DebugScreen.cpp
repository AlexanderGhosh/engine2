#include "DebugScreen.h"
#include "../GameObject/GameObject.h"

DebugScreen::DebugScreen() : mainCanvas(), fpsText(), counter(0)
{
}

void DebugScreen::awake()
{
	UI::Canvas* canvas = parent->getComponet<UI::Canvas>();
	if (NOT canvas) {
		parent->addComponet(&mainCanvas);
		canvas = &mainCanvas;
	}
	fpsText.setText("FPS: 1000");
	fpsText.setForgroundColor({ 1, 1, 1 });
	fpsText.setPos({ 115, 575 });
	canvas->addElement(fpsText);
}

void DebugScreen::update(float deltaTime)
{
	if (counter >= 1.5) {
		fpsText.setText("FPS: " + std::to_string(static_cast<int>(1.0f / deltaTime)));
		const float w = fpsText.getWidth() / 2.0f;
		const glm::vec2& p = fpsText.getPos();
		float diff = p.x - w; // left edge
		diff = 5 - diff;
		fpsText.setPos({ p.x + diff, p.y });
		counter = 0;
	}
	counter += deltaTime;
}

void DebugScreen::cleanUp()
{
	mainCanvas.cleanUp();
	fpsText.cleanUp();
}
