#pragma once
#include <ctime>

class Entity;

namespace msg {
	using msgType = std::size_t;

	enum MsgId : msgType {
		_GAME_START, //Cuando la ronda empieza
		_GAME_OVER, //Cuando la ronda acaba
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
		GameOverMsg(int won) : Message(_GAME_OVER), winner(won)
		{
		}

		int winner;
	};

	struct StartGameMsg : public Message {
		StartGameMsg() : Message(_GAME_START)
		{
		}
	};
}
