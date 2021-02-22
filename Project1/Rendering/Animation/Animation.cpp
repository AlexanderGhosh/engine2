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

Unsigned Render::Animation::Animation::getFrameCount() const
{
    return keyFrames.size();
}

void Render::Animation::Animation::addKeyFrame(const KeyFrame& frame)
{
    keyFrames.push_back(frame);
}

String Render::Animation::Animation::getName() const
{
    return name;
}

void Render::Animation::Animation::cleanUp()
{
    for (KeyFrame& frame : keyFrames) {
        frame.cleanUp();
    }
    keyFrames.clear();
}

void Render::Animation::KeyFrame::cleanUp()
{
    translations.clear();
}
