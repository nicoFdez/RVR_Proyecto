#include "SocketSystem.h"
#include "TronSystem.h"
#include "Manager.h"

SocketSystem::SocketSystem(const char * s, const char * p) 
: System(ecs::SysId::_sys_Socket), socket(s, p){
	
}

void SocketSystem::init() {

    //Thread que se encarga de la comunicacion con el server
	std::thread sim_thread([this] { listenMessages();});
	sim_thread.detach();
}

void SocketSystem::listenMessages(){

	while(true)
    {
        // //Recibir Mensajes de red
        TronServerMsg msg;
        socket.recv(msg);

        //Si el mensaje es de empezar partida se comunica el inicio al resto de sistemas
        if(msg.empezarPartida){
		    mngr_->send<msg::StartGameMsg>(); 
        }
        //Si el mensaje es de terminar partida se comunica el final al resto de sistemas
        else if(msg.terminarPartida){
			mngr_->send<msg::GameOverMsg>(msg.ganador);
        }
        //Si el mensaje es de volver al menu se comunica la vuelta al resto de sistemas
        else if(msg.backToMenu){
            mngr_->send<msg::Message>(msg::_ARRIVED_TO_MENU);
        }
        //Si el mensaje es de actualizacion se actualizan las variables correspondientes del TronSystem
        else{
            //Mando este mensaje para que aquellos sistemas que esten relacionados con el juego se actualicen con lo que ha dicho el server
			mngr_->send<msg::WorldState>(msg.tablero, msg.p1Pos,msg.p1Rot,msg.p2Pos,msg.p2Rot);
        }
    }
}

//Metodo llamado por otros sistemas para mandar un mensaje al servidor
void SocketSystem::sendToServer(Serializable& obj){
	socket.send(obj, socket);
}