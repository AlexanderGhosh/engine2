#include <gtx/matrix_decompose.hpp>
#include "Animation.h"

float Render::Animation::Animation::getScaleFactor(Float ticks, Float nextTS, Float lastTS) const
{
    float midWayLength = ticks - lastTS;
    float framesDiff = nextTS - lastTS;
    return midWayLength / framesDiff;
}

Render::Animation::Animation::Animation() : name(""), duration(0), keyFrames(), tps(0)
{
}

Render::Animation::Animation::Animation(String name, Float duration, Float tps) : name(name), duration(duration), keyFrames(), tps(tps)
{
}

const Render::Animation::KeyFrame Render::Animation::Animation::getFrame(Float timeInSecconds) const
{
    const float time = fmod(timeInSecconds * tps, duration);
    const Render::Animation::KeyFrame* first = &keyFrames[0];
    const Render::Animation::KeyFrame* seccond = &keyFrames[1];
    for (unsigned i = 0; i < keyFrames.size(); i++) {
        if (keyFrames[i].timeStamp > time) {
            seccond = &keyFrames[i];
            first = &keyFrames[(i - 1) % keyFrames.size()];
            break;
        }
    }
    float sf = getScaleFactor(time, seccond->timeStamp, first->timeStamp);
    return first->interpolate(*seccond, sf);
}

const Render::Animation::KeyFrame& Render::Animation::Animation::getFrameABS(Int frame) const
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

Float Render::Animation::Animation::getTPS() const
{
    return (tps) ? tps : 25.0f;
}

Float Render::Animation::Animation::getDuration() const
{
    return duration;
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

Render::Animation::KeyFrame Render::Animation::KeyFrame::interpolate(const KeyFrame& frame, Float ratio) const
{
    const int s = frame.translations.size();
    KeyFrame res;
    res.translations.reserve(s);
    res.translations.resize(s);
    for (short i = 0; i < s; i++) {
        glm::vec3 pos1, scale1, pos2, scale2, pos3, scale3;
        glm::quat rot1, rot2, rot3;
        glm::vec3 skew;
        glm::vec4 perspective;

        glm::decompose(translations[i], scale1, rot1, pos1, skew, perspective);
        glm::decompose(frame.translations[i], scale2, rot2, pos2, skew, perspective);

        pos3 = glm::mix(pos1, pos2, ratio);
        scale3 = glm::mix(scale1, scale2, ratio);
        rot3 = glm::slerp(rot1, rot2, ratio);

        glm::mat4 translation = glm::translate(glm::mat4(1), pos3);
        glm::mat4 scale = glm::scale(glm::mat4(1), scale3);
        glm::mat4 rotation = glm::mat4(rot3);

        res.translations[i] = translation * rotation * scale;
    }
    return res;
}
