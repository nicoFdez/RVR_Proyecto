#include "Serializable.h"
#include <vector>
#include "Vector2D.h"

class TronServerMsg: public Serializable
{

    //Esta clase va a representar el tipo de mensaje que va a utilizar el server para comunicarse con los clientes
    //Va a almacenar informacon general de la partida: mapa, jugadores y estado del juego (menu,jugando,game over)
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

    //Metodo heredado de serializable
    //Guarda la informacion del objeto serializado en _data
    void to_bin(){

        alloc_data(messageSize);
        memset(_data, 0, messageSize);

        //Metemos los datos del mapa
        char* aux = _data;
        for(int i=0;i<50;i++){
            for(int j=0;j<50;j++){
                memcpy(aux, &tablero[i][j], sizeof(int));
                aux += sizeof(int);
            }
        }

        //Metemos la info del player 1
        memcpy(aux, &p1Pos, sizeof(Vector2D));
        aux +=  sizeof(Vector2D);
        memcpy(aux, &p1Rot, sizeof(float));
        aux +=  sizeof(float);

        //Metemos la info del player 2
        memcpy(aux, &p2Pos, sizeof(Vector2D));
        aux +=  sizeof(Vector2D);    
        memcpy(aux, &p2Rot, sizeof(float));
        aux +=  sizeof(float);

        //Bools que representan los posibles estados de la partida (sus transiciones)
        memcpy(aux, &playersListos, sizeof(bool));
        aux +=  sizeof(bool); 
        memcpy(aux, &empezarPartida, sizeof(bool));
        aux +=  sizeof(bool); 
        memcpy(aux, &terminarPartida, sizeof(bool));
        aux +=  sizeof(bool);
        memcpy(aux, &backToMenu, sizeof(bool));
        aux +=  sizeof(bool);

        //Numero que representa el ganador de la partida
        memcpy(aux, &ganador, sizeof(int));

    }

    int from_bin(char * bobj){

        alloc_data(messageSize);
        memcpy(static_cast<void* >(_data), bobj, messageSize);

        //Sacamos los datos del mapa
        char* aux = _data;
        for(int i=0;i<50;i++){
            for(int j=0;j<50;j++){
                memcpy(&tablero[i][j],aux, sizeof(int));
                aux += sizeof(int);
            }
        }

        //Sacamos los datos del player1
        memcpy(&p1Pos,aux, sizeof(Vector2D));
        aux += sizeof(Vector2D);
        memcpy(&p1Rot,aux, sizeof(float));
        aux += sizeof(float);

        //Sacamos los datos del player2
        memcpy(&p2Pos,aux, sizeof(Vector2D));
        aux += sizeof(Vector2D);    
        memcpy(&p2Rot,aux, sizeof(float));
        aux += sizeof(float);  

        //Sacamos los bools que representan los posibles estados de la partida (sus transiciones)
        memcpy(&playersListos,aux, sizeof(bool));
        aux +=  sizeof(bool);
        memcpy(&empezarPartida,aux, sizeof(bool));
        aux +=  sizeof(bool); 
        memcpy(&terminarPartida,aux, sizeof(bool));
        aux +=  sizeof(bool);
        memcpy(&backToMenu,aux, sizeof(bool));
        aux +=  sizeof(bool);

        //Sacamos el jugador que ha ganado la partida
        memcpy(&ganador,aux, sizeof(int));

        return 0;
    }

    //Tamaño total del mensaje
    double messageSize;
    //Representacion del mapa de juego que se envia al cliente
    std::vector<std::vector<int>> tablero;

    //Posiciones y rotaciones de los jugadores
    Vector2D p1Pos;
    Vector2D p2Pos;
    float p1Rot;
    float p2Rot;

    //Flag que indica si ambos jugadores no estaban listos y ahora si
    bool playersListos = false;
    //Flag que indica a los clientes que comiencen la partida
    bool empezarPartida = false;
    //Flag que indica a los clientes que terminen la partida
    bool terminarPartida = false;
    //Flag que indica a los clientes que vuelvan al menu
    //Puede ser por haber terminado la partida o porque uno se ha desconectado
    bool backToMenu = false;

    //Ganador de la partida
    int ganador = 0;

};
