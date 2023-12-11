//
// Created by hum-s on 10/12/2023.
//

#ifndef SOCKET_SOCKET_H
#define SOCKET_SOCKET_H
#define DEFAULT_PORT "27015"
#define DEFAULT_BUF_LEN 512
#include <string>
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <iostream>

enum SocketType {Blocking, NonBlocking};

class Socket {
public:
    Socket();

    Socket(SOCKET s);
    Socket(const Socket&);

    virtual ~Socket();

    void close();

    std::string Recieve();

    void Send(std::string);

    Socket& operator=(Socket&);

private:
    static void start();
    static void end();
    static int nofSockets_;
protected:
    int* refCounter_;
    SOCKET socket_;

};

class SocketServer:public Socket{
public:
    SocketServer(SocketType type = Blocking);
    SocketServer(const std::string port, SocketType type = Blocking);
    Socket* Accept();

};

class SocketClient: public  Socket{
public:
    SocketClient (const std::string host, const std::string port);
    Socket* GetSocketPointer();
};




#endif //SOCKET_SOCKET_H
