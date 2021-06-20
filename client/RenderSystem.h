#pragma once
#include "GameState.h"
#include "System.h"
#include "Texture.h"


/*
* Sistema que se encarga del renderizado de la escena
*/
class RenderSystem : public System {
public:
	RenderSystem();
	void update() override;

private:
	//Metodo que se encarga de renderizar entidades que tienen animaciones
	void drawAnimated(Entity* e);

	//Metodo que se encarga de renderizar imagenes en determinadas posiciones
	void drawImage(Texture image, SDL_Rect renderRect);

	//Dibuja a ambos jugadores
	void drawTronPlayers(GameState* gs);

	//Dibuja las casillas por las que los jugadores han pasado
	void drawMap(GameState* gs);

	//Dibuja lo relacionado con texto
	void drawState(GameState* gs);
};
