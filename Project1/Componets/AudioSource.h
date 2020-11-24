#pragma once
#include "Componets.h"
#include "../Primatives/Buffers.h"
#include <al.h>

namespace Component {
    class AudioSource : public Base
    {
    private:
        unsigned source;
    public:
        AudioSource() : Base(), source(0) { };
        AudioSource(unsigned source) : Base(), source(source) { };
        void update();
        inline void play() const { alSourcePlay(source); };
        inline void pause() const { alSourcePause(source); };
        inline bool isPlaying() const 
        { 
            int res = 0;
            alGetSourcei(source, AL_SOURCE_STATE, &res);
            return res == AL_PLAYING;
        }
        void addBuffer(Primative::SoundBuffer* buffer) const;
        void cleanUp();
        inline Type getType() const { return Type::AudioSource; };
    };
};

