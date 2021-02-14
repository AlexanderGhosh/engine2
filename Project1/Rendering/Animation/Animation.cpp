#include "Animation.h"

Render::Animation::Animation::Animation() : name(""), duration(0), keyFrames() 
{
}

Render::Animation::Animation::Animation(String name, Float duration) : name(name), duration(duration), keyFrames()
{
}

const Render::Animation::KeyFrame& Render::Animation::Animation::getFrame(int frame) const
{
    return keyFrames[frame];
}

void Render::Animation::Animation::addKeyFrame(const KeyFrame& frame)
{
    keyFrames.push_back(frame);
}

String Render::Animation::Animation::getName() const
{
    return name;
}
