#include "Animated.h"

Component::Animated::Animated() : animations(), activeAnimation(""), currentClosestFrame(0)
{
}

void Component::Animated::startAnimation(const std::string& name)
{
	activeAnimation = name;
}

const Render::Animation::KeyFrame& Component::Animated::nextFrame()
{
	currentClosestFrame++;
	currentClosestFrame %= animations[activeAnimation].getFrameCount();
	return getCurrentFrame();
}

const Render::Animation::KeyFrame& Component::Animated::getCurrentFrame()
{
	return animations[activeAnimation].getFrame(currentClosestFrame);
}

void Component::Animated::addAnimation(const Render::Animation::Animation& animation)
{
	animations.insert(animations.end(), { animation.getName(), animation });
}

void Component::Animated::cleanUp()
{
	animations.clear();
}
