#include <thread>
#include "Chat.h"
#include "Tron.h"

void start(const char * s, const char * p) {
	//Inicializacion del cliente
	Tron g(s, p);

	//Ponemos a ejecutar
	g.start();
}

int main(int argc, char **argv)
{

    try {
        start(argv[1], argv[2]);

	} catch (std::string &e) { // catch errors thrown as strings
		cerr << e << endl;
	} catch (const char *e) { // catch errors thrown as char*
		cerr << e << endl;
	} catch (const std::exception &e) { // catch other exceptions
		cerr << e.what();
	} catch (...) {
		cerr << "Caught and exception of unknown type ..";
	}

	return 0;
}

