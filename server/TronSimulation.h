#pragma once

#include <vector>
#include "Socket.h"
#include "TronClientMessage.h"
#include "TronServerMsg.h"
#include <mutex>

using namespace std;

//Estado en el que se encuentra la partida que el server esta simulando
enum State : uint8_t { READY, RUNNING, OVER };

class TronSimulation {
public:
	TronSimulation(const char * s, const char * p);
	virtual ~TronSimulation();

	//Metodo que se encarga de la ejecucion/simulacion d ela partida en caso de tener los jugadores necesarios
	void run();

	//Thread que se encargara de recibir mensajes de los clientes
	void net_thread();

private:

	//Metodo que se encarga de preparar el sever para una partida de TRON
	void initGame();

	//Metodo que se encarga de esperar a que los 2 clientes que forman una partida esten conectados
	void waitForPlayers();

	//Metodo que se encarga de simular un ciclo de ejecucion de la partida
	void simulate();

	//Metodo que se encarga de desplazar un jugador (Utilizado por simulate)
	Vector2D updatePlayerPos(Vector2D playerPos, Vector2D dirPlayer);

	//Metodo que se encarga de comprobar colisiones en una posicion concreta (Utilizado por simulate)
	bool checkCollision(Vector2D pos);

	//Metodo que se encarga de resetear al mapa y a los jugadores
	void resetMatch();

	//Metodo que se hace cargo del reset de los jugadores
	void resetPlayers();

	//Metodo que se encarga de procesari el posible input que haya mandado el Jugador 1
	void proccessP1Input();

	//Metodo que se encarga de procesari el posible input que haya mandado el Jugador 2
	void proccessP2Input();

	//Estado de la partida simulada
	State state = State::READY;


	//Socket que representa al jugador1
	Socket* player1=nullptr;

	//Socket que representa al jugador2
	Socket* player2=nullptr;

	//Ultima pulsacion realizada por el jugador 1
	TronClientMsg::MsgType lastPressedP1 = TronClientMsg::MsgType::NONE;

	//Ultima pulsacion realizada por el jugador 2
	TronClientMsg::MsgType lastPressedP2 = TronClientMsg::MsgType::NONE;


    /*
     * Socket del servidor
     */
    Socket mySocket;

	//Mapa del juego
	vector<vector<int>> encasillado;

	//Datos del player 1
	Vector2D posP1;
	Vector2D dirP1;
	float rotP1;

	//Datos del player 2
	Vector2D posP2;
	Vector2D dirP2;
	float rotP2;

	//Mutex para controlar el input
	std::mutex playerInputMutex;
};
