#include "Animation.h"

Render::Animation::Animation::Animation() : name(""), duration(0), keyFrames() 
{
}

Render::Animation::Animation::Animation(String name, Float duration) : name(name), duration(duration), keyFrames()
{
}

const void* Render::Animation::Animation::getNextFrame()
{
    return nullptr;
}

void Render::Animation::Animation::addKeyFrame(const KeyFrame& frame)
{
    keyFrames.push_back(frame);
}
