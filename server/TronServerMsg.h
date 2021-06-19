#include "Serializable.h"
#include <vector>
#include "Vector2D.h"

class TronServerMsg: public Serializable
{
public:

    TronServerMsg(vector<vector<int>> encasillado, Vector2D p1P, Vector2D p2P, float p1R, float p2R){
        tablero = encasillado;
        p1Pos = p1P;
        p2Pos = p2P;
        p1Rot = p1R;
        p2Rot = p2R;
        messageSize = ((50*50)*sizeof(int)) + (2*sizeof(Vector2D))+ (2*sizeof(float)) + (4*sizeof(bool))+ sizeof(int);
    };

    TronServerMsg(){
        tablero = vector<vector<int>> (50, vector<int>(50,0));
        messageSize = ((50*50)*sizeof(int)) + (2*sizeof(Vector2D))+ (2*sizeof(float)) + (4*sizeof(bool)) + sizeof(int);
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

        memcpy(aux, &p1Pos, sizeof(Vector2D));
        aux +=  sizeof(Vector2D);
        memcpy(aux, &p1Rot, sizeof(float));
        aux +=  sizeof(float);
        memcpy(aux, &p2Pos, sizeof(Vector2D));
        aux +=  sizeof(Vector2D);    
        memcpy(aux, &p2Rot, sizeof(float));
        aux +=  sizeof(float);

        memcpy(aux, &playersListos, sizeof(bool));
        aux +=  sizeof(bool); 
        memcpy(aux, &empezarPartida, sizeof(bool));
        aux +=  sizeof(bool); 
        memcpy(aux, &terminarPartida, sizeof(bool));
        aux +=  sizeof(bool);
        memcpy(aux, &backToMenu, sizeof(bool));
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

        memcpy(&p1Pos,aux, sizeof(Vector2D));
        aux += sizeof(Vector2D);
        memcpy(&p1Rot,aux, sizeof(float));
        aux += sizeof(float);
        memcpy(&p2Pos,aux, sizeof(Vector2D));
        aux += sizeof(Vector2D);    
        memcpy(&p2Rot,aux, sizeof(float));
        aux += sizeof(float);  

        memcpy(&playersListos,aux, sizeof(bool));
        aux +=  sizeof(bool);
        memcpy(&empezarPartida,aux, sizeof(bool));
        aux +=  sizeof(bool); 
        memcpy(&terminarPartida,aux, sizeof(bool));
        aux +=  sizeof(bool);
        memcpy(&backToMenu,aux, sizeof(bool));
        aux +=  sizeof(bool);

        memcpy(&ganador,aux, sizeof(int));

        return 0;
    }

    double messageSize;
    std::vector<std::vector<int>> tablero;

    Vector2D p1Pos;
    Vector2D p2Pos;
    float p1Rot;
    float p2Rot;

    bool playersListos = false;
    bool empezarPartida = false;
    bool terminarPartida = false;
    bool backToMenu = false;
    int ganador = 0;

};
