//
// Created by hum-s on 09/12/2023.
//
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <iostream>


#include "../Socket.h"
#include <list>
typedef  std::list<Socket*> socket_list;
socket_list connections;
unsigned __stdcall Connection(void* a){
    Socket* s = (Socket*) a;

    connections.push_back(s);

    s->Send("Conexion establecida");
    std::cout << "Se ha conectado un nuevo cliente"<<'\n';

    while(true){
        std::string response = s->Recieve();

        if(response == "exit()") {
            s->Send("exit()");
            break;
        }


        for(socket_list ::iterator destination = connections.begin(); destination!=connections.end(); destination++){

            if(*destination != s) {
                (*destination)->Send(response);
                std::cout << "Reenviado: " << response << '\n';
                std::cout.flush();
            }
        }
    }
    s->Send("exit()");
    std::cout << "Cerrando una conexion" << '\n';
    connections.remove(s);
    delete s;
    return 0;
}

int main(int argc, char **argv){
    SocketServer server("3000");
    std::cout << "Servidor de mensajeria iniciado en el puerto 3000\n";

    while(1){
        Socket* s = server.Accept();
        unsigned ret;
        _beginthreadex(0,0,Connection,(void*)s, 0, &ret);
    }
}
