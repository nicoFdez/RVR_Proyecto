#pragma once

#include "ecs.h"

//Clase padre de los componentes de nuestro juego
struct Component {
	Component(ecs::CmpIdType id) :
			id_(id) {
	}

	//ID del componente
	ecs::CmpIdType id_;
};

