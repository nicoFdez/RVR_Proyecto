#pragma once
#include <ctime>

class Entity;

namespace msg {

using msgType = std::size_t;

enum MsgId : msgType {
	_GAME_START, //Cuando la ronda empieza
	_GAME_OVER, //Cuando la ronda acaba
	_PLAYER_DEATH, //Cuando el jugador muere
	_ARRIVED_TO_MENU, //Cuando llega al menu principal
	//
	_last_MsgId_
};

struct Message {
	Message(MsgId id) :
			id(id) {
	}

	MsgId id;
};

struct GameOverMsg : public Message {
	GameOverMsg(bool won) : Message(_GAME_OVER), won(won) 
	{
	}

	bool won; //Cuando acaba la ronda, indica si el jugador gano o no
};


struct StartGameMsg : public Message {
	StartGameMsg(size_t nGhosts, size_t nCherries) : Message(_GAME_START), nGhosts(nGhosts), nCherries(nCherries)
	{
	}

	size_t nGhosts, nCherries; //el numero de fantasmas | cerezas iniciales
};

}
