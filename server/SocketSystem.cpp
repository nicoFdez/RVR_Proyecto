#include "SocketSystem.h"
#include "TronSystem.h"
#include "Manager.h"

SocketSystem::SocketSystem(const char * s, const char * p) 
: System(ecs::SysId::_sys_Socket), socket(s, p){
	
}

void SocketSystem::init() {
	std::thread sim_thread([this] { listenMessages();});
	sim_thread.detach();
}

void SocketSystem::update() {
	
}

void SocketSystem::receive(const msg::Message& msg)
{


	
}

void SocketSystem::listenMessages(){
	while(true)
    {
        // //Recibir Mensajes de red
        TronServerMsg msg;
        socket.recv(msg);

        if(msg.empezarPartida){
		    mngr_->send<msg::StartGameMsg>(); 
        }
        else if(msg.terminarPartida){
			mngr_->send<msg::GameOverMsg>(msg.ganador);
        }
        else{
            TronSystem* tSystem = mngr_->getSystem<TronSystem>(ecs::SysId::_sys_Tron);
            tSystem->setEncasillado(msg.tablero);
            tSystem->setPlayerTransform(1,msg.tr1);
            tSystem->setPlayerTransform(2,msg.tr2);
        }
    }
}

void SocketSystem::sendToServer(Serializable& obj){
	socket.send(obj, socket);
}