// #pragma once

// #include "Transform.h"
// #include "Serializable.h"

// class TronSystem : public System {
// public:
// 	enum estadoCasilla { none, player1, player2 };
// 	struct casilla {
// 		estadoCasilla miEstado = estadoCasilla::none;
// 		SDL_Rect renderRect;
// 	};
// 	TronSystem();
// 	void init() override;
// 	void update() override;
// 	void inputManagement();
// 	Vector2D updatePlayerPos(Transform* trPlayer, Vector2D dirPlayer);
// 	bool checkCollision(Vector2D pos);
// 	virtual void receive(const msg::Message& msg) override;

// 	Entity* getPlayer1() { return player1_; }
// 	Entity* getPlayer2() { return player2_; }

// 	const vector<vector<casilla>>& getTronMap() { return encasillado; }
// 	void reset();

// 	void setEncasillado(vector<vector<int>> mapa);

// 	void setPlayerTransform(int id, Transform tr);

// private:
// 	Entity* player1_;
// 	Entity* player2_;

// 	Vector2D _dirP1;
// 	Vector2D _dirP2;
// 	Transform* tr1_;
// 	Transform* tr2_;

// 	double tamCas;

// 	Key::keyType lastKeyPressed;

// 	vector<vector<casilla>> encasillado;

// 	double movementTimer;
// 	double lastTickMoved;
// };
