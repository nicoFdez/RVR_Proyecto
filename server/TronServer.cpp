#include <thread>
#include "Chat.h"
#include "TronSimulation.h"

void start(const char * s, const char * p) {
	TronSimulation g(s, p);
}

int main(int argc, char **argv)
{
	start(argv[1], argv[2]);
	return 0;
}




