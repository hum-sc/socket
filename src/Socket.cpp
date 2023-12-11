//
// Created by hum-s on 10/12/2023.
//

#include "Socket.h"


int Socket::nofSockets_ = 0;

void Socket::start() {
    if(!nofSockets_){
        WSAData wsaData;
        int iResult = WSAStartup( MAKEWORD(2,2), &wsaData);

        if( iResult != 0 ){
            throw "No se pudo iniciar WSA";
        }
    }
    nofSockets_++;
}

void Socket::end() {
    WSACleanup();
}

Socket::Socket(): socket_(0) {
    start();
    socket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if(socket_ == INVALID_SOCKET){
        throw "Socket invalido";
    }
    refCounter_ = new int(1);
}

Socket::Socket(SOCKET s): socket_(s) {
    start();
    refCounter_ = new int(1);
}

Socket::~Socket() {
    if(!--(*refCounter_)){
        close();
        delete refCounter_;
    }

    --nofSockets_;
    if(!nofSockets_) end();
}


Socket::Socket(const Socket& o) {
    refCounter_=o.refCounter_;
    (*refCounter_)++;
    socket_=o.socket_;

    nofSockets_++;
}

Socket& Socket::operator=(Socket& o) {
    (*o.refCounter_)++;
    refCounter_ = o.refCounter_;

    socket_ = o.socket_;

    nofSockets_++;
    return *this;
}
void Socket::close() {
    closesocket(socket_);
}

std::string Socket::Recieve() {
    char recvbuf[DEFAULT_BUF_LEN];
    std::string line = "";
    int iResult = recv(socket_, recvbuf, DEFAULT_BUF_LEN, 0);
    if(iResult == 0) {
        return line;
    } else if(iResult == -1){
        return "";
    }
    line = recvbuf;
    return line;
}

void Socket::Send(std::string s) {
    std::cout<<s<<" local";
    send(socket_, s.c_str(),s.length(), 0);
}

SocketServer::SocketServer(SocketType type) {

    struct addrinfo *result = NULL, hints;
    ZeroMemory(&hints, sizeof (hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    if(socket_ == INVALID_SOCKET){
        throw "Socket invalido";
    }

    if(type == NonBlocking){
        u_long buflen = DEFAULT_BUF_LEN;
        ioctlsocket(socket_, FIONBIO, &buflen);
    }

    int iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);

    if(iResult != 0) throw "Error en getaddrinfo: "+iResult;

    iResult = bind(socket_, result->ai_addr, (int) result->ai_addrlen);

    if( iResult == SOCKET_ERROR){
        freeaddrinfo(result);
        closesocket(socket_);
        throw "Error al conectar el socket en bind()";
    }

    listen(socket_, SOMAXCONN);
}

SocketServer::SocketServer(const std::string port, SocketType type){

    struct addrinfo *result = NULL, hints;
    ZeroMemory(&hints, sizeof (hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    if(socket_ == INVALID_SOCKET){
        throw "Socket invalido";
    }

    if(type == NonBlocking){
        u_long buflen = DEFAULT_BUF_LEN;
        ioctlsocket(socket_, FIONBIO, &buflen);
    }

    int iResult = getaddrinfo(NULL, port.c_str(), &hints, &result);

    if(iResult != 0) throw "Error en getaddrinfo: "+iResult;

    iResult = bind(socket_, result->ai_addr, (int) result->ai_addrlen);

    if( iResult == SOCKET_ERROR){
        freeaddrinfo(result);
        closesocket(socket_);
        throw "Error al conectar el socket en bind()";
    }

    listen(socket_, SOMAXCONN);

    freeaddrinfo(result);

}

Socket *SocketServer::Accept() {
    SOCKET client_socket = accept(socket_, NULL, NULL);

    if(client_socket == INVALID_SOCKET){
        int rc = WSAGetLastError();
        if(rc == WSAEWOULDBLOCK){
            return 0;
        } else {
            throw "Socket invalido";
        }
        closesocket(socket_);
        throw "Error al aceptar la connexión: "+WSAGetLastError();
    }

    return new Socket(client_socket);
}

SocketClient::SocketClient(const std::string host, const std::string port):Socket() {
    struct addrinfo *result = NULL,
            *ptr = NULL,
            hints;

    ZeroMemory(&hints, sizeof (hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    SOCKET connectSocket = INVALID_SOCKET;

    int iResult = getaddrinfo(host.c_str(), port.c_str(), &hints, &result);
    if(iResult != 0){
        throw("Error en getaddrinfo");
    }
    for(ptr = result; ptr != NULL; ptr = ptr->ai_next){
        connectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if(connectSocket == INVALID_SOCKET) throw "Error al crear el socket"+WSAGetLastError();

        iResult = connect(connectSocket, ptr->ai_addr, (int) ptr->ai_addrlen);

        if(iResult == SOCKET_ERROR){
            closesocket(connectSocket);
            connectSocket = INVALID_SOCKET;
            continue;
        }
        socket_ = connectSocket;
        break;
    }

    freeaddrinfo(result);

    if(connectSocket == INVALID_SOCKET){
        throw "No se pudo conectar al servidor :(";
    }
}

Socket *SocketClient::GetSocketPointer() {
    return new Socket(socket_);
}

