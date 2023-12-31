//
// Created by hum-s on 10/12/2023.
//

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <thread>
#include <iostream>
#include "../Socket.h"

using namespace  std;
void SenderProccess(Socket s){
    std::string data;
    do{
        data = "";
        getline(cin, data);
        s.Send(data);
    }while(data != "exit()");
}

void RecieverProccess(Socket s){
    while(true){
        std::string recievedLine = "";
        recievedLine = s.Recieve();
        if(recievedLine == "") continue;
        if(recievedLine=="exit()")break;
        std::cout <<"R: "<< recievedLine<<'\n';
        cout.flush();
    }
}

int main(){

    string host, port;
    cout << "Introduce la direccion del host... \n";
    cin >> host;
    cout << "Introduce el numero de puerto...\n";
    cin >> port;
    try{
        SocketClient s(host, port);
        thread senderThread(SenderProccess, s);
        thread recieverThread(RecieverProccess, s);
        senderThread.join();
        recieverThread.join();
        cout<<"fin";

    }catch(const char* s){
        cerr << s<<endl;
    }
    catch (string s){
        cerr<<s<<endl;
    }
    catch(...){
        cerr<<"Error desconocido"<<endl;
    }
    return 0;
}