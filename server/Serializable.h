#ifndef SERIALIZABLE_H_
#define SERIALIZABLE_H_

#include <stdlib.h>
#include <cstring>

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

/**
 *  Este interfaz debe implementarlo aquellos objetos que se transmitan por red
 *  Define una serialización sencilla para la transmisión:
 *
 *  +--------+
 *  |        |  Esta es la representación binaria del objeto. Cada objeto
 *  |        |  deberá implementar dos métodos:
 *  |  data  |    - to_bin() para rellenar
 *  |        |    - from_bin() para reconstruirse a partir de data.
 *  |        |    - size es el tamaño total de la región data
 *  +--------+
 */
class Serializable
{
public:

    Serializable():_size(0), _data(0){};

    virtual ~Serializable()
    {
        if ( _data != 0 )
        {
            free(_data);
        }
    }

    /**
     *  Genera la representación binaria de la clase. Debe inicializar
     *  el buffer interno con la función helper alloc_data.
     */
    virtual void to_bin() = 0;

    /**
     *  Esta función recibe un objeto serializado y lo reconstruye.
     *    @param data representación binaria del objeto
     *    @return 0 si éxito -1 en caso contrario
     */
    virtual int from_bin(char * data) = 0;

    /**
     *  Devuelve un puntero al buffer interno con la representación del objeto.
     *  Debe inicializarse previamente via Serializable::to_bin()
     *    @return objeto serializado
     */
    char * data()
    {
        return _data;
    }

    /**
     *  @return tamaño del objeto serializado
     */
    int32_t size()
    {
        return _size;
    }

protected:

    int32_t _size;

    char *  _data;

    /**
     *  Reserva memoria para el buffer del objeto serializado
     */
    void alloc_data(int32_t data_size)
    {
        if ( _data != 0 )
        {
            free(_data);
        }

        _data = (char *) malloc(data_size);
        _size = data_size;
    }
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class Key: public Serializable
{
public:

    //Enum que represena las opciones que tiene el cliente para comunicarse con el server
    enum keyType
    {
        LEFT   = 0,
        RIGHT = 1,
        DOWN  = 2,
        UP  = 3,
        ESC  = 4,
        ENTER  = 5,
        NONE = 6
    };

    Key(){};

    Key(keyType keyPressed):key(keyPressed),connect(false){};

    //Metodo heredado que guarda el objeto serializado en _data
    void to_bin(){
        int size = sizeof(keyType)+ sizeof(bool);
        alloc_data(size);
        memset(_data, 0, size);

        //Serializar los campos type, nick y message en el buffer _data
        char* aux = _data;
        memcpy(aux, &key, sizeof(keyType));
        aux+=sizeof(keyType);
        memcpy(aux, &connect, sizeof(bool));
    }

    //Metodo heredado de serializable para sacar la informacion de datos binarios
    int from_bin(char * bobj){
        int size = sizeof(keyType)+ sizeof(bool);
        alloc_data(size);

        memcpy(static_cast<void *>(_data), bobj, size);

        //Reconstruir la clase usando el buffer _data
        char* aux = _data;
        memcpy(&key, aux, sizeof(keyType));
        aux+=sizeof(keyType);
        memcpy(&connect, aux, sizeof(bool));

        return 0;
    }

    //Flag utilizado por los clientes que sirve para indicar si pretende conectarse al server
    bool connect=false;

    //Tipo de mensaje que el cliente pretende comunicar al servidor
    keyType key=  keyType::NONE;
};


#endif /* SERIALIZABLE_H_ */
