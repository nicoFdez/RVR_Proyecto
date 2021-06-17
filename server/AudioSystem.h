#pragma once
#include "System.h"

class AudioSystem : public System
{
public:
	AudioSystem();
	virtual void receive(const msg::Message& msg);
};

