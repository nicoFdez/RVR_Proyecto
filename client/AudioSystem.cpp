#include "AudioSystem.h"
#include "Resources.h"
#include "SDLGame.h"

AudioSystem::AudioSystem(): System(ecs::_sys_Audio)
{
}

void AudioSystem::receive(const msg::Message& msg)
{
	switch (msg.id)
	{
		//Dependiendo del mensaje que nos llegue hacemos una accion u otra con el audio/musica
		case msg::_GAME_OVER: {
	 		game_->getAudioMngr()->playChannel(Resources::Collision, 0);
			game_->getAudioMngr()->haltMusic();
	 		break;
		}
		case msg::_GAME_START: {
			game_->getAudioMngr()->haltChannel();
			game_->getAudioMngr()->playMusic(Resources::Music);
			break;
		}
		default:
			break;
	}
}
