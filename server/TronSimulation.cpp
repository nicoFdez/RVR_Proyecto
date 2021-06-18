#include "TronSimulation.h"
#include <assert.h>
#include <unistd.h>
#include <thread>

#define TICKRATE 300000

using namespace std;

TronSimulation::TronSimulation(const char * s, const char * p) : mySocket(Socket(s,p)), player1(nullptr), player2(nullptr),
	exit_(false) {
	
	mySocket.bind();
	encasillado = vector<vector<int>>(50, vector<int>(50));
	initGame();
	std::thread sim_thread([this] { net_thread();});
	sim_thread.detach();

	run();
}

TronSimulation::~TronSimulation() {
	closeGame();
}

void TronSimulation::initGame() {
	while( player1 == nullptr ) {

		if(player1 == nullptr) {
			Key receptor;
			Socket* nuevo = (Socket*)&receptor;    //Hago que el puntero apunte a cualquier cosa para que el recv me cree un nuevo socket
			mySocket.recv(receptor,nuevo);
			if(receptor.connect){
				player1 = nuevo;
			}
		}
		// else if(player2 == nullptr) {
		// 	Key receptor;
		// 	Socket* nuevo = (Socket*)&receptor;    //Hago que el puntero apunte a cualquier cosa para que el recv me cree un nuevo socket
		// 	mySocket.recv(receptor,nuevo);
		// 	if(receptor.key == Key::keyType::NONE){
		// 		player2 = nuevo;
		// 	}
		// }
	}


	cout << "HURRAAA SE CONECTA EL P1\n";
	
	dirP1 = Vector2D(0,1);
	dirP2 = Vector2D(0,-1);
	rotP1= 90;
	rotP2 = 270;
	posP1 = Vector2D(50/4 , 50/4);
	posP2= Vector2D(50*3/4 , 50*3/4);
	
	TronServerMsg inicio; 
	inicio.empezarPartida = true;
	mySocket.send(inicio,*player1);
}

void TronSimulation::closeGame() {

}

void TronSimulation::run() {

	while(player1 != nullptr ){
		usleep(TICKRATE);
		proccessP1Input();
		proccessP2Input();
		simulate();
	}
}


void TronSimulation::simulate(){

	posP1 = updatePlayerPos(posP1, dirP1);
	posP2 = updatePlayerPos(posP2, dirP2);

	TronServerMsg actualizacion;
	if(checkCollision(posP1)){
		actualizacion.terminarPartida = true;
		actualizacion.ganador = 2; 
	}
	else if(checkCollision(posP2)){
		actualizacion.terminarPartida = true;
		actualizacion.ganador = 1; 
	}
	else{
		encasillado[posP1.getX()][posP1.getY()] = 1;
		encasillado[posP2.getX()][posP2.getY()] = 2;
		actualizacion.p1Pos = posP1;
		actualizacion.p2Pos = posP2;
		actualizacion.p1Rot = rotP1;
		actualizacion.p2Rot = rotP2;
		actualizacion.tablero = encasillado;
	}
	mySocket.send(actualizacion,*player1);

}



Vector2D TronSimulation::updatePlayerPos(Vector2D playerPos, Vector2D dirPlayer)
{
	// Movimiento del player 1
	Vector2D oldPositions = playerPos;
	playerPos = playerPos + dirPlayer;

	int x = playerPos.getX();
	int y = playerPos.getY();

	if (x < 0 || x  >= 50 || y < 0 || y  >= 50) {
		cout<<"Player se ha salido\n";
		playerPos = oldPositions;
	}

	return playerPos;
}

bool TronSimulation::checkCollision(Vector2D pos)
{
	return encasillado[pos.getX()][pos.getY()] != 0;
}



void TronSimulation::proccessP1Input(){
	if(lastPressedP1 != Key::keyType::NONE){
		switch (lastPressedP1){
			case Key::keyType::UP:{
				rotP1=0;
				dirP1.setX(-1);
				dirP1.setY(0);
			break;
			}
			case Key::keyType::DOWN:{
				rotP1=180;
				dirP1.setX(1);
				dirP1.setY(0);
			break;
			}
			case Key::keyType::LEFT:{
				rotP1=270;
				dirP1.setX(0);
				dirP1.setY(-1);
			break;
			}
			case Key::keyType::RIGHT:{
				rotP1=90;
				dirP1.setX(0);
				dirP1.setY(1);
			break;
			}
		}
	lastPressedP1 = Key::keyType::NONE;
	}
}

void TronSimulation::proccessP2Input(){
}




void TronSimulation::net_thread(){

	while(true)
    {
        // //Recibir Mensajes de red
        Key msg;
        Socket* nuevo = (Socket*)&msg;    //Hago que el puntero apunte a cualquier cosa para que el recv me cree un nuevo socket
        mySocket.recv(msg,nuevo);


		if(nuevo == player1){
			lastPressedP1 = msg.key;
		}
		else if(nuevo == player2){
			lastPressedP2 = msg.key;
		}

    }
}