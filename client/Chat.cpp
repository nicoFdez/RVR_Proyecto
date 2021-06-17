#include "Chat.h"
#include "InputHandler.h"

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

void ChatServer::listen_clients()
{
    while (true)
    {
        /*
         * NOTA: los clientes están definidos con "smart pointers", es necesario
         * crear un unique_ptr con el objeto socket recibido y usar std::move
         * para añadirlo al vector
         */

        Socket* clientSd;
        Key k;
        //Recibir Mensajes en y en función del tipo de mensaje
        socket.recv(k, clientSd);
        // - LOGIN: Añadir al vector clients
        if(k.key == Key::keyType::ENTER){
            std::cout << "Client pressed enter\n";
        }
    }
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

void ChatClient::login()
{
    // std::string msg;

    // ChatMessage em(nick, msg);
    // em.type = ChatMessage::LOGIN;

    // socket.send(em, socket);
}

void ChatClient::logout()
{
    // std::string msg;

    // ChatMessage em(nick, msg);
    // em.type = ChatMessage::LOGOUT;

    // socket.send(em, socket);
}

void ChatClient::input_thread()
{
    while (true)
    {
        auto ih = InputHandler::instance();
        if (ih->keyDownEvent()) {
            if (ih->isKeyDown(SDLK_RIGHT)) {
                
            }
            else if (ih->isKeyDown(SDLK_LEFT)) {
                
            }
            else if (ih->isKeyDown(SDLK_UP)) {
                
            }
            else if (ih->isKeyDown(SDLK_DOWN)) {
                
            }
            else if (ih->isKeyDown(SDLK_RETURN)) {
                Key k(Key::keyType::ENTER);
                cout << "Enter pulsado\n";
                // Enviar al servidor usando socket
                socket.send(k, socket);
            }
            else if (ih->isKeyDown(SDLK_ESCAPE)) {
                
            }
            cout << "Tecla pulsada\n";
        }
    }
}

void ChatClient::net_thread()
{
    while(true)
    {
        // //Recibir Mensajes de red
        // ChatMessage chatMsg;
        // socket.recv(chatMsg);
        // //Mostrar en pantalla el mensaje de la forma "nick: mensaje"
        // std::cout << chatMsg.nick << ": " << chatMsg.message << "\n";
    }
}

