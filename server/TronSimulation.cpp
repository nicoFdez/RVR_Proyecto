#include "TronSimulation.h"
#include <assert.h>
#include <unistd.h>
#include <thread>

//Tiempo entre actualizaciones del juego
#define TICKRATE 500000

using namespace std;

TronSimulation::TronSimulation(const char * s, const char * p) : mySocket(Socket(s,p)), player1(nullptr), player2(nullptr){
	
	//Preparamos nuestro socket
	mySocket.bind();

	//Preparamos la partida 
	encasillado = vector<vector<int>>(50, vector<int>(50));
	initGame();

	//Preparamos el thread de networking
	std::thread sim_thread([this] { net_thread();});
	sim_thread.detach();

	//Llamamos a run para comenzar la partida
	run();
}

TronSimulation::~TronSimulation() {
}

void TronSimulation::initGame() {
	
	waitForPlayers();
	resetMatch();
}


void TronSimulation::run() {

	//Mientras haya jugadores en el server continuamos
	while(!(player1 == nullptr && player2 == nullptr)){

		//Solo continuamos la partida si ambos jugadores estan conectados correctamente
		if(!(player1 == nullptr || player2 == nullptr)){
			usleep(TICKRATE);

			playerInputMutex.lock();
			proccessP1Input();
			proccessP2Input();
			playerInputMutex.unlock();

			if(state == State::RUNNING)
				simulate();
		}
	}
}


void TronSimulation::waitForPlayers(){

	while(player1 == nullptr || player2 == nullptr){
		//Esperar a que alguien ocupe el puesto del player1
		while(player1 == nullptr){
			Key receptor;
			Socket* nuevo;   
			mySocket.recv(receptor,nuevo);
			if(receptor.connect){
				if(player2 != nullptr && !(*nuevo == *player2)){
					player1 = nuevo;
				}
				else if(player2 == nullptr)
					player1 = nuevo;
			}
		}

		//Esperar a que alguien ocupe el puesto del player2
		while(player2 == nullptr){
			Key receptor;
			Socket* nuevo;   
			mySocket.recv(receptor,nuevo);
			if(receptor.connect){
				if(player1 != nullptr && !(*nuevo == *player1)){
					player2 = nuevo;
				}
				else if(player1 == nullptr)
					player2 = nuevo;
			}
		}
	}
}



void TronSimulation::simulate(){

	//Actualizacion de posiciones
	posP1 = updatePlayerPos(posP1, dirP1);
	posP2 = updatePlayerPos(posP2, dirP2);

	//Comprobacion de colisiones
	TronServerMsg actualizacion;
	if(checkCollision(posP1)){
		actualizacion.terminarPartida = true;
		actualizacion.ganador = 2;
		state = State::OVER;
		resetMatch();
	}
	else if(checkCollision(posP2)){
		actualizacion.terminarPartida = true;
		actualizacion.ganador = 1;
		state = State::OVER;
		resetMatch();
	}
	//Si no hay colisiones actualizamos mapa 
	else{
		encasillado[posP1.getX()][posP1.getY()] = 1;
		encasillado[posP2.getX()][posP2.getY()] = 2;
		actualizacion.p1Pos = posP1;
		actualizacion.p2Pos = posP2;
		actualizacion.p1Rot = rotP1;
		actualizacion.p2Rot = rotP2;
		actualizacion.tablero = encasillado;
	}

	//Avisamos a los clientes de lo que ha pasado
	mySocket.send(actualizacion,*player1);
	mySocket.send(actualizacion,*player2);
}

Vector2D TronSimulation::updatePlayerPos(Vector2D playerPos, Vector2D dirPlayer)
{

	//Actalizamos la posicion que nos han dado teniendo en cuenta la direccion que nos han dado
	Vector2D oldPositions = playerPos;
	playerPos = playerPos + dirPlayer;
	int x = playerPos.getX();
	int y = playerPos.getY();

	//si el resultado se sale de los limites del mapa lo devolvemos a su posicion inicial
	if (x < 0 || x  >= 50 || y < 0 || y  >= 50) {
		playerPos = oldPositions;
	}

	return playerPos;
}

bool TronSimulation::checkCollision(Vector2D pos)
{
	//Comprueba si una casilla determinada ya es controlada por un player o no
	return encasillado[pos.getX()][pos.getY()] != 0;
}

void TronSimulation::proccessP1Input(){

	//En caso de que haya inpt del player1
	if(lastPressedP1 != Key::keyType::NONE){
		switch (lastPressedP1){

			//Actualizamos direcciones
			case Key::keyType::UP:{
				rotP1=0;
				dirP1.setX(0);
				dirP1.setY(-1);
			break;
			}
			case Key::keyType::DOWN:{
				rotP1=180;
				dirP1.setX(0);
				dirP1.setY(1);
			break;
			}
			case Key::keyType::LEFT:{
				rotP1=270;
				dirP1.setX(-1);
				dirP1.setY(0);
			break;
			}
			case Key::keyType::RIGHT:{
				rotP1=90;
				dirP1.setX(1);
				dirP1.setY(0);
			break;
			}
			
			//Si detectamos en enter y no estamos en el estado RUNNING realizamos la transicion correcta
			case Key::keyType::ENTER:{
				if(state == State::READY){
					state = State::RUNNING;
					TronServerMsg inicio; 
					inicio.empezarPartida = true;
					mySocket.send(inicio,*player1);
					mySocket.send(inicio,*player2);
				}
				else if(state == State::OVER){
					state = State::READY;
					TronServerMsg reinicio; 
					reinicio.backToMenu = true;
					mySocket.send(reinicio,*player1);
					mySocket.send(reinicio,*player2);
				}
			break;
			}

			//En caso de esc el player se ha ido y colvemos al menu
			case Key::keyType::ESC:{
				player1 = nullptr;
				state = State::READY;
				TronServerMsg menu; 
				menu.backToMenu = true;
				resetMatch();
				if(player2 != nullptr)
					mySocket.send(menu,*player2);
			break;
			}
		}
	lastPressedP1 = Key::keyType::NONE;
	}
}

void TronSimulation::proccessP2Input(){

	//En caso de que haya inpt del player2
	if(lastPressedP2 != Key::keyType::NONE){
		switch (lastPressedP2){

			//Actualizamos direcciones
			case Key::keyType::UP:{
				rotP2=0;
				dirP2.setX(0);
				dirP2.setY(-1);
			break;
			}
			case Key::keyType::DOWN:{
				rotP2=180;
				dirP2.setX(0);
				dirP2.setY(1);
			break;
			}
			case Key::keyType::LEFT:{
				rotP2=270;
				dirP2.setX(-1);
				dirP2.setY(0);
			break;
			}
			case Key::keyType::RIGHT:{
				rotP2=90;
				dirP2.setX(1);
				dirP2.setY(0);
			break;
			}

			//Si detectamos en enter y no estamos en el estado RUNNING realizamos la transicion correcta
			case Key::keyType::ENTER:{
				if(state == State::READY){
					state = State::RUNNING;
					TronServerMsg inicio; 
					inicio.empezarPartida = true;
					mySocket.send(inicio,*player1);
					mySocket.send(inicio,*player2);
				}
				else if(state = State::OVER){
					state = State::READY;
					TronServerMsg reinicio; 
					reinicio.backToMenu = true;
					mySocket.send(reinicio,*player1);
					mySocket.send(reinicio,*player2);
				}
			break;
			}

			//En caso de esc el player se ha ido y colvemos al menu
			case Key::keyType::ESC:{
				player2 = nullptr;
				state = State::READY;
				TronServerMsg menu; 
				menu.backToMenu = true;
				resetMatch();
				if(player1 != nullptr)
					mySocket.send(menu,*player1);
			break;
			}
		}	
		lastPressedP2 = Key::keyType::NONE;
	}
}

void TronSimulation::net_thread(){

	while(true)
    {
		//Recibimos un mensaje de algun cliene
        Key msg;
        Socket* nuevo = (Socket*)&msg;    
        mySocket.recv(msg,nuevo);

		//Si es una conexion lo ponemos en el primer luar que quede libre
		if(msg.connect){

			//Lo ponemos en el lugar del player1
			if(player1 == nullptr){
				if(player2 != nullptr && !(*nuevo == *player2)){
					player1 = nuevo;
				}
				else if(player2 == nullptr)
					player1 = nuevo;
			}

			//Lo ponemos en el lugar del player2
			else if(player2 == nullptr){
				if(player1 != nullptr && !(*nuevo == *player1)){
					player2 = nuevo;
				}
				else if(player1 == nullptr)
					player2 = nuevo;
			}
		}

		//Si no es de conexion es de input y se actualiza la tecla del player que me haya mandado el mensaje
		else{
			playerInputMutex.lock();
			if(player1 != nullptr && *nuevo == *player1){
				lastPressedP1 = msg.key;
			}
			else if(player2 != nullptr && *nuevo == *player2){
				lastPressedP2 = msg.key;
			}
			playerInputMutex.unlock();
		}

    }
}

void TronSimulation::resetMatch(){

	//Reset del estado mapa
	for(int i=0; i<encasillado.size(); i++){
		for(int j=0; j<encasillado[i].size(); j++){
			encasillado[i][j] = 0;
		}
	}
	resetPlayers();
}

void TronSimulation::resetPlayers(){

	//Informacion del player1
	posP1 = Vector2D(50/4 , 50/4);
	dirP1.setX(1);
	dirP1.setY(0);
	rotP1= 90;
	lastPressedP1 = Key::keyType::NONE;

	//Informacion del player2
	posP2= Vector2D(50*3/4 , 50*3/4);
	dirP2.setX(-1);
	dirP2.setY(0);
	rotP2 = 270;
	lastPressedP2 = Key::keyType::NONE;

}