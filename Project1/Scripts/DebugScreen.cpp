#include "DebugScreen.h"
#include "../Scene/GameScene.h"
#include "../GameObject/GameObject.h"

DebugScreen::DebugScreen(Vector3 colour) : mainCanvas(), fpsText(), counter(0), position(), screenDim(-1), bgColour(colour)
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
	fpsText.setForgroundColor(bgColour);
	fpsText.setPos({ 115, 0 });
	canvas->addElement(fpsText);
	position.setText("Position: " + Utils::to_string_precision(parent->getTransform()->Position, 2));
	position.setForgroundColor(bgColour);
	position.setPos({ 115, 0 });
	canvas->addElement(position);
}

void DebugScreen::update(float deltaTime)
{
	if (screenDim.x < 0) {
		screenDim = parent->getScene()->getScreenDimentions();
		position.setPos({ 115, screenDim.y - 50 });
		fpsText.setPos({ 115, screenDim.y - 15 });
	}
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

	position.setText("Position: " + Utils::to_string_precision(parent->getTransform()->Position, 2));
	const float w = position.getWidth() / 2.0f;
	const glm::vec2& p = position.getPos();
	float diff = p.x - w; // left edge
	diff = 5 - diff;
	position.setPos({ p.x + diff, p.y });
}

void DebugScreen::cleanUp()
{
	mainCanvas.cleanUp();
	fpsText.cleanUp();
	position.cleanUp();
}
