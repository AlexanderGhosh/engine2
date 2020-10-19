#include "Animation.h"

const void* Render::Animation::Animation::getNextFrame()
{
    currentFrame++;
    currentFrame = currentFrame % frameCount;
    return nullptr;
}
