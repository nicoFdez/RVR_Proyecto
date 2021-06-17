#include "Chat.h"
#include <iostream>
#include "Tron.h"

void start() {
	Tron g;
	g.start();
}

int main(int argc, char **argv)
{
    ChatServer es(argv[1], argv[2]);
	//es.listen_clients();

    try {
		start();
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
