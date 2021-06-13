#pragma once

#include "System.h"

class FoodSystem: public System {
public:
	FoodSystem();
	void init() override;
	void update() override;
	virtual void receive(const msg::Message& msg) override;

private:
	void addFood(std::size_t n);
	void onEat(Entity *e);
	void disableAll();
	int numOfFoodPieces_;

};

