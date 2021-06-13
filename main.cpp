#include <iostream>

#include "PacMan.h"

using namespace std;

void start() {
	PacMan g;
	g.start();
}

int main(int ac, char **av) {

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
