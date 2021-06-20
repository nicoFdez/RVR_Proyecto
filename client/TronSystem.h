#pragma once

#include "System.h"
#include "Transform.h"
#include "Entity.h"
#include "ImageComponent.h"
#include "Serializable.h"

class TronSystem : public System {
public:

	//Enum que vaos a usar para indicar el estado de las casillas del juego
	enum estadoCasilla { none, player1, player2 };

	//Struct que va a representar cada casilla
	struct casilla {
		estadoCasilla miEstado = estadoCasilla::none;
		SDL_Rect renderRect;
	};


	TronSystem();
	//Inicializacion
	void init() override;
	//Actualizacion del cliente (Input management)
	void update() override;
	//Se recoge el input del jugador y se manda el correspondiente mensaje al servidor
	void inputManagement();


	//Metodo virtual qye define la manera en la que este sistema recibe mensajes del resto
	virtual void receive(const msg::Message& msg) override;

	//Devuelve la entidad que representa al player 1
	Entity* getPlayer1() { return player1_; }

	//Devuelve la entidad que representa al player 2
	Entity* getPlayer2() { return player2_; }

	//Devuelve el mapa del juego
	const vector<vector<casilla>>& getTronMap() { return encasillado; }

	//Metodo que sirve para dejar el mapa en un estado que nos indiquen
	//Se utiliza para cuando el server nos pasa la informacion del mundo
	void setEncasillado(vector<vector<int>> mapa);

	//Metodo utilizado para establecer el estado de un jugador concreto
	//Este metodo se usa cuando el server nos manda la informacion del mundo
	void setPlayerTransform(int id, Vector2D pos, float rot );

private:
	//Entidad que representa al player1
	Entity* player1_;

	//Entidad que representa al player1
	Entity* player2_;

	//Ultima Key que ha pulsado el player
	Key::keyType lastKeyPressed;

	//Tamaño de la casilla de nuestro mapa
	double tamCas;

	//Vector que reresenta el mapa del juego
	vector<vector<casilla>> encasillado;

	//Direccion que sigue el player1
	Vector2D _dirP1;

	//Direccion que sigue el player2
	Vector2D _dirP2;

	//Transform del player1
	Transform* tr1_;

	//Transform del player1
	Transform* tr2_;
};
