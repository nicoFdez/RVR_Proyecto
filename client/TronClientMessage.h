#include <stdlib.h>
#include <cstring>
#include "Serializable.h"


class TronClientMsg: public Serializable
{
public:

    //Enum que represena las opciones que tiene el cliente para comunicarse con el server
    enum MsgType
    {
        LEFT   = 0,
        RIGHT = 1,
        DOWN  = 2,
        UP  = 3,
        ESC  = 4,
        ENTER  = 5,
        NONE = 6
    };

    TronClientMsg(){};

    TronClientMsg(MsgType keyPressed):msg(keyPressed),connect(false){};

    //Metodo heredado que guarda el objeto serializado en _data
    void to_bin(){
        int size = sizeof(MsgType)+ sizeof(bool);
        alloc_data(size);
        memset(_data, 0, size);

        //Serializar los campos type, nick y message en el buffer _data
        char* aux = _data;
        memcpy(aux, &msg, sizeof(MsgType));
        aux+=sizeof(MsgType);
        memcpy(aux, &connect, sizeof(bool));
    }

    //Metodo heredado de serializable para sacar la informacion de datos binarios
    int from_bin(char * bobj){
        int size = sizeof(MsgType)+ sizeof(bool);
        alloc_data(size);

        memcpy(static_cast<void *>(_data), bobj, size);

        //Reconstruir la clase usando el buffer _data
        char* aux = _data;
        memcpy(&msg, aux, sizeof(MsgType));
        aux+=sizeof(MsgType);
        memcpy(&connect, aux, sizeof(bool));

        return 0;
    }

    //Flag utilizado por los clientes que sirve para indicar si pretende conectarse al server
    bool connect=false;

    //Tipo de mensaje que el cliente pretende comunicar al servidor
    MsgType msg=  MsgType::NONE;
};
