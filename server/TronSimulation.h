#pragma once

#include <vector>
#include "TronSystem.h"
#include "SocketSystem.h"
#include "Socket.h"
#include "Serializable.h"
#include "TronServerMsg.h"
#include <mutex>

using namespace std;

enum State : uint8_t { READY, RUNNING, OVER };

class TronSimulation {
public:
	TronSimulation(const char * s, const char * p);
	virtual ~TronSimulation();
	void run();

	void net_thread();
private:

	void waitForPlayers();
	void initGame();

	void simulate();
	Vector2D updatePlayerPos(Vector2D playerPos, Vector2D dirPlayer);
	bool checkCollision(Vector2D pos);

	void resetMatch();
	void resetPlayers();

	void proccessP1Input();
	void proccessP2Input();

	bool exit_;
	State state = State::READY;


	Socket* player1=nullptr;
	Socket* player2=nullptr;

	Key::keyType lastPressedP1 = Key::keyType::NONE;
	Key::keyType lastPressedP2 = Key::keyType::NONE;


    /**
     * Socket del servidor
     */
    Socket mySocket;

	vector<vector<int>> encasillado;
	Vector2D posP1;
	Vector2D posP2;
	Vector2D dirP1;
	Vector2D dirP2;
	float rotP1;
	float rotP2;
	std::mutex playerInputMutex;
};
