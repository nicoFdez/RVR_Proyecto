#include <thread>
#include "Chat.h"
#include "Tron.h"

void start() {
	Tron g;
	g.start();
}

int main(int argc, char **argv)
{
    //ChatServer es(argv[1], argv[2]);
	//es.listen_clients();

    try {

        ChatClient ec(argv[1], argv[2]);
        std::thread net_thread([&ec](){ start();});
		ec.input_thread();

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

/*int main(int argc, char **argv)
{
    //Ponemos a ejecutar
    Tron a;
    a.start();
    //std::thread net_thread([&a](){ a.start(); });

    //Escuchamos posible input
    // ChatClient ec(argv[1], argv[2]);
    // ec.input_thread();
    
    //std::thread net_thread([&ec](){ ec.net_thread(); });

    //ec.login();


    return 0;
}*/

