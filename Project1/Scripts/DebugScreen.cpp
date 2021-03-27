#include "DebugScreen.h"
#include "../Scene/GameScene.h"
#include "../GameObject/GameObject.h"
#include "../Componets/RigidBody.h"

DebugScreen::DebugScreen() : mainCanvas(), fpsText(), counter(0), position(), screenDim(-1), redBlockPosition(), RedBlock(nullptr), redBlockVelocity()
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
	fpsText.setPos({ 115, 0 });
	canvas->addElement(fpsText);
	position.setText("Position: " + Utils::to_string_precision(parent->getTransform()->Position, 2));
	position.setForgroundColor({ 1, 1, 1 });
	position.setPos({ 115, 0 });
	canvas->addElement(position);

	if (RedBlock) {
		redBlockPosition.setText("Red Block Position: " + Utils::to_string_precision(RedBlock->getTransform()->Position, 2));
		redBlockPosition.setForgroundColor({ 1, 1, 1 });
		redBlockPosition.setPos({ 115, 0 });
		canvas->addElement(redBlockPosition);

		redBlockVelocity.setText("Red Block Velocity: " + Utils::to_string_precision(RedBlock->getRigidbody()->getVelocity(), 2));
		redBlockVelocity.setForgroundColor({ 1, 1, 1 });
		redBlockVelocity.setPos({ 115, 0 });
		canvas->addElement(redBlockVelocity);
	}
}

void DebugScreen::update(float deltaTime)
{
	if (screenDim.x < 0) {
		screenDim = parent->getScene()->getScreenDimentions();
		const int start = 15;
		const int increment = 35;
		fpsText.         setPos({ 115, screenDim.y - (start + increment * 0)});
		position.        setPos({ 115, screenDim.y - (start + increment * 1)});
		redBlockPosition.setPos({ 115, screenDim.y - (start + increment * 2)});
		redBlockVelocity.setPos({ 115, screenDim.y - (start + increment * 3)});
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

	if (RedBlock) {
		redBlockPosition.setText("Red Block Position: " + Utils::to_string_precision(RedBlock->getTransform()->Position, 2));
		float w_ = redBlockPosition.getWidth() / 2.0f;
		const glm::vec2& p_ = redBlockPosition.getPos();
		float diff_ = p_.x - w_; // left edge
		diff_ = 5 - diff_;
		redBlockPosition.setPos({ p_.x + diff_, p_.y });

		redBlockVelocity.setText("Red Block Velocity: " + Utils::to_string_precision(RedBlock->getRigidbody()->getVelocity(), 2));
		w_ = redBlockVelocity.getWidth() / 2.0f;
		const glm::vec2 p_1 = redBlockVelocity.getPos();
		diff_ = p_1.x - w_; // left edge
		diff_ = 5 - diff_;
		redBlockVelocity.setPos({ p_1.x + diff_, p_1.y });
	}
}

void DebugScreen::cleanUp()
{
	mainCanvas.cleanUp();
	fpsText.cleanUp();
	position.cleanUp();
	RedBlock = nullptr;
	redBlockPosition.cleanUp();
	redBlockVelocity.cleanUp();
}
