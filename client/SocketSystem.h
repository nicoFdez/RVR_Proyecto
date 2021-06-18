#pragma once

#include "System.h"
#include "Socket.h"
#include <thread>
#include "Serializable.h"

class SocketSystem : public System {
public:
	SocketSystem(const char * s, const char * p);
	void init() override;
	void update() override;
	virtual void receive(const msg::Message& msg) override;
	void sendToServer(Serializable& obj, const Socket& sock);

private:
	Socket socket;
	static void listenMessages();
};
