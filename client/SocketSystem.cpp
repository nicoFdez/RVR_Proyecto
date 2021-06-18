#include "SocketSystem.h"

SocketSystem::SocketSystem(const char * s, const char * p) 
: System(ecs::SysId::_sys_Socket), socket(s, p){
	
}

void SocketSystem::init() {
	std::thread sim_thread([](){listenMessages();});
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
        // TronServerMsg chatMsg;
        // socket.recv(chatMsg);
        // //Mostrar en pantalla el mensaje de la forma "nick: mensaje"
        // std::cout << chatMsg.nick << ": " << chatMsg.message << "\n";
    }
}

void SocketSystem::sendToServer(Serializable& obj, const Socket& sock){
	socket.send(obj, sock);
}