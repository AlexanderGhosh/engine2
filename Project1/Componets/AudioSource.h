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
        bool isLooping;
    public:
        AudioSource();
        AudioSource(unsigned source);
        void update(float deltaTime);
        void play() const;
        void pause();
        void togglePlay();
        void toggleLoop();
        bool isPlaying() const;
        void addBuffer(Primative::Buffers::SoundBuffer* buffer) const;
        void cleanUp();
        inline Type getType() const { return Type::AudioSource; };
    };
};

