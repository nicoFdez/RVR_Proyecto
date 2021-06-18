#include "Serializable.h"
#include <vector>
#include "Transform.h"

class TronServerMsg: public Serializable
{
public:

    TronServerMsg(vector<vector<int>> encasillado, Transform t1, Transform t2){
        tablero = encasillado;
        tr1 = t1;
        tr2 = t2;
        messageSize = ((50*50)*sizeof(int)) + (2*sizeof(Transform)) + (2*sizeof(bool))+ sizeof(int);
    };

    TronServerMsg(){
        messageSize = ((50*50)*sizeof(int)) + (2*sizeof(Transform)) + (2*sizeof(bool))+ sizeof(int);
    };


    void to_bin(){

        alloc_data(messageSize);

        memset(_data, 0, messageSize);

        //Serializar los campos type, nick y message en el buffer _data
        char* aux = _data;
        for(int i=0;i<50;i++){
            for(int j=0;j<50;j++){
                memcpy(aux, &tablero[i][j], sizeof(int));
                aux += sizeof(int);
            }
        }

        memcpy(aux, &tr1, sizeof(Transform));
        aux +=  sizeof(Transform);
        memcpy(aux, &tr2, sizeof(Transform));
        aux +=  sizeof(Transform);    

        memcpy(aux, &empezarPartida, sizeof(bool));
        aux +=  sizeof(bool); 
        memcpy(aux, &terminarPartida, sizeof(bool));
        aux +=  sizeof(bool);   

        memcpy(aux, &ganador, sizeof(int));

    }

    int from_bin(char * bobj){

        alloc_data(messageSize);
        memcpy(static_cast<void* >(_data), bobj, messageSize);

        //Serializar los campos type, nick y message en el buffer _data
        char* aux = _data;
        for(int i=0;i<50;i++){
            for(int j=0;j<50;j++){
                memcpy(&tablero[i][j],aux, sizeof(int));
                aux += sizeof(int);
            }
        }

        memcpy( &tr1,aux, sizeof(Transform));
        aux +=  sizeof(Transform);
        memcpy(&tr2,aux, sizeof(Transform));
        aux +=  sizeof(Transform);    

        memcpy(&empezarPartida,aux, sizeof(bool));
        aux +=  sizeof(bool); 
        memcpy(&terminarPartida,aux, sizeof(bool));
        aux +=  sizeof(bool);   

        memcpy(&ganador,aux, sizeof(int));

        return 0;
    }

    double messageSize;
    std::vector<std::vector<int>> tablero;
    Transform tr1;
    Transform tr2;

    bool empezarPartida;
    bool terminarPartida;
    int ganador;

};
