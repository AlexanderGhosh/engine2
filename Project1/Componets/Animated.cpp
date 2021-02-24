#include "Animated.h"
#include "../Rendering/Animation/Animation.h"

Component::Animated::Animated() : animations(), activeAnimation(""), runTime(0), running(false)
{
}

void Component::Animated::startAnimation(const std::string& name, bool start)
{
	if (NOT animations.size())
		return;
	activeAnimation = name;
	running = start;
}

void Component::Animated::stopAnimation()
{
	running = false;
}

void Component::Animated::playAnimation()
{
	running = false;
}

void Component::Animated::togglePlay()
{
	running = NOT running;
}

const Render::Animation::KeyFrame& Component::Animated::nextFrame()
{
	/*runTime++;
	runTime %= animations[activeAnimation].getFrameCount();*/
	return getCurrentFrame();
}

const Render::Animation::KeyFrame Component::Animated::getCurrentFrame()
{
	if (NOT running OR NOT getActiveAnimation())
		return {};
	return getActiveAnimation()->getFrame(runTime);
}

void Component::Animated::addAnimation(Render::Animation::Animation* animation)
{
	if(animation)
		animations.insert(animations.end(), { animation->getName(), animation });
}

void Component::Animated::cleanUp()
{
	animations.clear();
}

void Component::Animated::update(float deltaTime)
{
	if (running AND getActiveAnimation()) {
		runTime += deltaTime;
		runTime = fmod(runTime, getActiveAnimation()->getDuration());
	}
}

const Render::Animation::Animation* Component::Animated::getActiveAnimation()
{
	const int s = animations.size();
	auto res = animations[activeAnimation];
	if (s != animations.size()) {
		animations.erase(activeAnimation);
		return nullptr;
	}
	return res;
}
