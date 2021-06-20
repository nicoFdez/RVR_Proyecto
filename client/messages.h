#pragma once
#include <ctime>
#include<vector>
#include "Vector2D.h"

class Entity;

namespace msg {
	using msgType = std::size_t;

	enum MsgId : msgType {
		_GAME_START, //Cuando la ronda empieza
		_GAME_OVER, //Cuando la ronda acaba
		_ARRIVED_TO_MENU, //Cuando llega al menu principal
		_WORLD_STATE, //Cuando el server nos informa del estado de la partida
		//
		_last_MsgId_
	};


	//Structs que se mandan entre sistemas para comunicar mensajes
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

	struct WorldState : public Message {
		WorldState(std::vector<std::vector<int>> mundo,Vector2D pP1, float rP1, Vector2D pP2, float rP2) : 
		Message(_WORLD_STATE), tablero(mundo), p1Pos(pP1),p1Rot(rP1), p2Pos(pP2),p2Rot(rP2)
		{
		}
		std::vector<std::vector<int>> tablero;
		Vector2D p1Pos;
		float p1Rot;
		Vector2D p2Pos;
		float p2Rot;
	};
}
