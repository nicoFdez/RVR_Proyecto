#pragma once

#include <vector>

#include "GameCtrlSystem.h"
#include "Manager.h"
#include "TronSystem.h"
#include "RenderSystem.h"
#include "SDLGame.h"
#include "AudioSystem.h"
#include "SocketSystem.h"

class Tron {
public:

	//Constuctor de la clase
	Tron(const char * s, const char * p);
	virtual ~Tron();

	// Metodo que pone al cliente a ejecutar el bucle principal
	void start();

private:
	//Metodo que sirve para inicializar SDL y aquellos sistemas que sean necesarios para la ejecucion
	void initGame(const char * s, const char * p);
	
	//Liberacion de los recursos
	void closeGame();

	//Manager que se hace cargo de todo lo relacionado con SDL
	SDLGame* game_;

	//Manager que se hace cargo de todos los sistemas del juego
	Manager* mngr_;

	//True cuando se debe cerrar el juego
	bool exit_;

	//Sistemas que componen el juego
	RenderSystem* renderSystem_;
	TronSystem* tronSystem_;
	GameCtrlSystem* gameCtrlSystem_;
	AudioSystem* audioSystem_;
	SocketSystem* socketSystem_;

	//Tamaño de la pantalla
	const static int _WINDOW_WIDTH_ = 500;
	const static int _WINDOW_HEIGHT_ = 500;
};
