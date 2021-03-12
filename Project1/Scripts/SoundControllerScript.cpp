#include "SoundControllerScript.h"
#include "../EventSystem/Handler.h"
#include "../GameObject/GameObject.h"
#include "../Componets/AudioSource.h"

using EH = Events::Handler;
using Key = Events::Key;
using Cursor = Events::Cursor;
using Action = Events::Action;

SoundControllerScript::SoundControllerScript() : Component::Scripting()
{
}

void SoundControllerScript::keyButton(float deltaTime)
{
	Component::AudioSource* source = parent->getComponet<Component::AudioSource>();
	if (NOT source) {
		return;
	}
	if (EH::getKey(Key::P, Action::Down)) {
		source->play();
	}
	if (EH::getKey(Key::Q, Action::Down)) {
		source->pause();
	}
	if (EH::getKey(Key::Enter, Action::Down)) {
		std::cout << std::to_string(source->isPlaying()) << std::endl;
	}
	if (EH::getKey(Key::L, Action::Down)) {
		source->toggleLoop();
	}
}
