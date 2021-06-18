#include "Serializable.h"
#include <vector>
#include "Transform.h"

class TronServerMsg: public Serializable
{
public:

    TronServerMsg(){};

    void to_bin(){
        // int size = sizeof(keyType);
        // alloc_data(size);
        // memset(_data, 0, size);

        // //Serializar los campos type, nick y message en el buffer _data
        // char* aux = _data;
        // memcpy(aux, &key, size);
    }

    int from_bin(char * bobj){
        // int size = sizeof(keyType);
        // alloc_data(size);

        // memcpy(static_cast<void *>(_data), bobj, size);

        // //Reconstruir la clase usando el buffer _data
        // char* aux = _data;
        // memcpy(&key, aux, size);

        return 0;
    }

    std::vector<std::vector<int>> tablero;
    Transform tr1;
    Transform tr2;
};
