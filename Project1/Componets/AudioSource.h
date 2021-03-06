#pragma once
#include "Componets.h"
#include <al.h>

namespace Primative {
    namespace Buffers {
        class SoundBuffer;
    }
}
namespace Component {
    class AudioSource : public ComponetBase
    {
    private:
        unsigned source;
    public:
        AudioSource() : ComponetBase(), source(0) { };
        AudioSource(unsigned source) : ComponetBase(), source(source) { };
        void update(float deltaTime);
        inline void play() const { alSourcePlay(source); };
        inline void pause() const { alSourcePause(source); };
        inline bool isPlaying() const 
        { 
            int res = 0;
            alGetSourcei(source, AL_SOURCE_STATE, &res);
            return res == AL_PLAYING;
        }
        void addBuffer(Primative::Buffers::SoundBuffer* buffer) const;
        void cleanUp();
        inline Type getType() const { return Type::AudioSource; };
    };
};

