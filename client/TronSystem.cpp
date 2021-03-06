#include "TronSystem.h"

#include "InputHandler.h"
#include "Manager.h"
#include "SDLGame.h"
#include "AnimatedImageComponent.h"
#include "GameState.h"
#include "SDL_macros.h"
#include "GameCtrlSystem.h"
#include "SocketSystem.h"

TronSystem::TronSystem() :
	System(ecs::_sys_Tron), ///
	player1_(nullptr), //
	player2_(nullptr),
	tr1_(nullptr) {
}

void TronSystem::init() {
	//Se establece dimensiones generales de la casilla
	tamCas = game_->getWindowWidth()/50;
	encasillado = vector <vector<casilla>>(50, vector<casilla>(50, casilla()));

	//Inicialmente el player n ha pulsado nada
	lastKeyPressed = TronClientMsg::MsgType::NONE;

	//Establecemos posiciones de los Rects
	for (int i = 0; i < encasillado.size(); i++) {
		for (int j = 0; j < encasillado.size(); j++) {
			encasillado[i][j].renderRect = RECT(i * tamCas, j * tamCas, tamCas, tamCas);
		}
	}
	
	//Entidades de los jugadores
	player1_ = mngr_->addEntity();
	player2_ = mngr_->addEntity();

	//Transforms de los jugadores
	tr1_ = player1_->addComponent<Transform>();
	tr1_->width_ = tr1_->height_ = 10;
	tr2_ = player2_->addComponent<Transform>();
	tr2_->width_ = tr2_->height_ = 10;

	//Animacion del player1
	auto animatedImage = player1_->addComponent<AnimatedImageComponent>();
	animatedImage->setFrameTime(100);
	Texture* spritesTex = game_->getTextureMngr()->getTexture(
		Resources::PacManSprites);
	for (int i = 0; i < 4; i++) {
		animatedImage->addFrame(spritesTex, { i * 128, 0, 128, 128 });
	}

	//Animacion del player2
	animatedImage = player2_->addComponent<AnimatedImageComponent>();
	animatedImage->setFrameTime(100);
	spritesTex = game_->getTextureMngr()->getTexture(
		Resources::PacManSprites);
	for (int i = 0; i < 4; i++) {
		animatedImage->addFrame(spritesTex, { i * 128, 0, 128, 128 });
	}
}

void TronSystem::update() {

	//Si estamos en estado RUNNING (jugando) preguntamos por input sino, no hacemos nada
	auto gameState = mngr_->getSystem<GameCtrlSystem>(ecs::SysId::_sys_GameCtrl)->getGameStateEntity()->getComponent<GameState>(ecs::GameState);
	if (gameState->state_ != GameState::RUNNING)
		return;

	inputManagement();
}

void TronSystem::inputManagement()
{
	//Se recoge la tecla pulsada por el jugador
	lastKeyPressed = TronClientMsg::MsgType::NONE;
	auto ih = InputHandler::instance();
	if (ih->keyDownEvent()) {
		if (ih->isKeyDown(SDLK_RIGHT)) {
			lastKeyPressed = TronClientMsg::MsgType::RIGHT;
		}
		else if (ih->isKeyDown(SDLK_LEFT)) {
			lastKeyPressed = TronClientMsg::MsgType::LEFT;
		}
		else if (ih->isKeyDown(SDLK_UP)) {
			lastKeyPressed = TronClientMsg::MsgType::UP;
		}
		else if (ih->isKeyDown(SDLK_DOWN)) {
			lastKeyPressed = TronClientMsg::MsgType::DOWN;
		}
		//Mandar mensaje con tecla pulsada
		TronClientMsg message(lastKeyPressed);
 		mngr_->getSystem<SocketSystem>(ecs::SysId::_sys_Socket)->sendToServer(message);	
	}
}

void TronSystem::receive(const msg::Message& msg)
{
	
	switch (msg.id)
	{
		//Si el mensaje esta relacionado con la partida lo casteo y seteo el mapa y los 
		//jugadores a lo que me hayan indicado
		case msg::_WORLD_STATE: {
			msg::WorldState estado = static_cast<const msg::WorldState&>(msg);
		 	setEncasillado(estado.tablero);
            setPlayerTransform(1,estado.p1Pos, estado.p1Rot);
            setPlayerTransform(2,estado.p2Pos,estado.p2Rot);
			break;
		}
		default:
			break;
	}
}

void TronSystem::setEncasillado(vector<vector<int>> mapa){
	//Establecemos las casillas a los valores que nos den
	for(int i=0;i<mapa.size();i++){
		for(int j=0;j<mapa.size();j++){
			encasillado[i][j].miEstado = (estadoCasilla)mapa[i][j];
		}
	}

}

void TronSystem::setPlayerTransform(int id, Vector2D pos, float rot){

	//Seteamos los valores del player 1
	if(id == 1){
		tr1_->position_ = (pos*tamCas);
		tr1_->rotation_ = rot;
	}

	//Seteamos los valores del player 2
	else if(id == 2){
		tr2_->position_ = (pos*tamCas);
		tr2_->rotation_ = rot;
	}
}
