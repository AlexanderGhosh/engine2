#include "Animated.h"

Component::Animated::Animated() : animations(), activeAnimation(""), runTime(0)
{
}

void Component::Animated::startAnimation(const std::string& name)
{
	activeAnimation = name;
}

const Render::Animation::KeyFrame& Component::Animated::nextFrame()
{
	/*runTime++;
	runTime %= animations[activeAnimation].getFrameCount();*/
	return getCurrentFrame();
}

const Render::Animation::KeyFrame Component::Animated::getCurrentFrame()
{
	return animations[activeAnimation].getFrame(runTime);
}

void Component::Animated::addAnimation(const Render::Animation::Animation& animation)
{
	animations.insert(animations.end(), { animation.getName(), animation });
}

void Component::Animated::cleanUp()
{
	animations.clear();
}

void Component::Animated::update(float deltaTime)
{
	runTime += deltaTime;
}
