#pragma once
#include "System.h"


/*
* Sistema que se encarga de la reproduccion de audio durante la ejecucion
*/
class AudioSystem : public System
{
public:
	AudioSystem();
	virtual void receive(const msg::Message& msg);
};

