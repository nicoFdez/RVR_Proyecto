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
	case msg::_PAC_MAN_DEATH: { 
		game_->getAudioMngr()->haltMusic();
		game_->getAudioMngr()->playChannel(Resources::PacMan_Death, 0);
		break;
	}
	case msg::_NO_MORE_FOOD: {
		game_->getAudioMngr()->haltMusic();
		game_->getAudioMngr()->playChannel(Resources::PacMan_Won,0);
		break;
	}
	case msg::_COLLISION_WITH_CHERRY: {
		game_->getAudioMngr()->playChannel(Resources::PacMan_Eat, 0);
		break;
	}
	case msg::_GAME_START: {
		game_->getAudioMngr()->haltMusic();
		break;
	}
	case msg::_ARRIVED_TO_MENU: {
		game_->getAudioMngr()->haltChannel();
		game_->getAudioMngr()->playMusic(Resources::PacMan_Intro);
		break;
	}
	default:
		break;
	}
}
