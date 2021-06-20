#pragma once

#include "System.h"
#include "Socket.h"
#include <thread>
#include "Serializable.h"
#include "TronServerMsg.h"


/*
* Sistema que nos permite controlar la comucacion entre el server y el cliente
*/
class SocketSystem : public System {
public:

	//Constructor de la clase, prepara ek socket con el que nos vamos a comunicar
	SocketSystem(const char * s, const char * p);

	//Comienza la ejecucion del thread que se va a comunicar con el server
	void init() override;

	//Metodo usado para mandar mensajes al servidor desde otros sistemas
	void sendToServer(Serializable& obj);

private:
	//Socket utilizado por el cliente
	Socket socket;
	//Metodo que sirve para comunicarse con el server (va a ser ejecutado por un thread
	void listenMessages();
};
