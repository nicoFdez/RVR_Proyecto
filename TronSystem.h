#pragma once

#include "System.h"
#include "Transform.h"
#include "Entity.h"

class TronSystem: public System {
public:
	//enum estadoCasilla {none, player1, player2};
	TronSystem();
	void init() override;
	void update() override;
	virtual void receive(const msg::Message& msg) override;

private:
	void resetPacManPosition();
	Entity *player1_;
	Entity *player2_;

	//double tamCas;
	
	// struct casilla{
	// 	estadoCasilla miEstado = estadoCasilla::none;
	// };

	// Vector2D _dirP1;
	// Vector2D _dirP2;
	Transform *tr1_;
};

