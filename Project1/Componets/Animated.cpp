#include "Animated.h"

Component::Animated::Animated() : animations(), skeleton(), activeAnimation(""), currentClosestFrame(0)
{
}

void Component::Animated::startAnimation(const std::string& name)
{
	activeAnimation = name;
}

Render::Animation::KeyFrame Component::Animated::nextFrame()
{
	currentClosestFrame++;
	return animations[activeAnimation].getFrame(currentClosestFrame);
}

void Component::Animated::setSkeleton(const Render::Animation::Skeleton& skeleton)
{
	this->skeleton = skeleton;
}

void Component::Animated::addAnimation(const Render::Animation::Animation& animation)
{
	animations.insert(animations.end(), { animation.getName(), animation });
}
