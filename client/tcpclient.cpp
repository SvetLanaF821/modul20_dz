#include "tcpclient.h"
#include <iostream>
#include <string>

TcpClient::TcpClient() : sockets(-1), serveraddr(), client() {
    bzero(m, lengthMessage);
}

//настроить клиент
void TcpClient::config(const char* address, uint16_t port) {
    sockets = socket(AF_INET, SOCK_STREAM, 0);          //создать сокет

    if (sockets == -1) {
        throw "Creation of socket failed";
    }

    serveraddr.sin_addr.s_addr = inet_addr(address);    //установить адрес сервера
    serveraddr.sin_port = htons(port);                  //задать номер порта
    serveraddr.sin_family = AF_INET;                    //использовать IPv4
}

//соединение с сервером
void TcpClient::connectToServer() {
    //установить соединение с сервером
    int connection = connect(sockets, (struct sockaddr*)&serveraddr, sizeof(serveraddr));

    if (connection == -1) {
        throw "Connect with server failed";
    }
}

//отправить сообщение
bool TcpClient::send(const char* message) {
    bzero(m, sizeof(m));
    strcpy(m, message);
    ssize_t bytes = write(sockets, m, sizeof(m));

    //если передать получилось, то возвращаем true, иначе - false
    return bytes > 0;
}

//отправить сообщение
bool TcpClient::send(const int n) {
    bzero(m, sizeof(m));
    std::string temp = std::to_string(n);
    strcpy(m, temp.c_str());
    ssize_t bytes = write(sockets, m, sizeof(m));

    //если передать получилось, то возвращаем true, иначе - false
    return bytes > 0;
}

//отправить сообщение
bool TcpClient::send(const unsigned n) {
    bzero(m, sizeof(m));
    std::string temp = std::to_string(n);
    strcpy(m, temp.c_str());
    ssize_t bytes = write(sockets, m, sizeof(m));
    //если передать получилось, то возвращаем true, иначе - false

    return bytes > 0;
}

//отправить сообщение
bool TcpClient::send(const bool bit) {
    bzero(m, sizeof(m));
    if (bit) {
        strcpy(m, "1");
    }
    else {
        strcpy(m, "0");
    }
    ssize_t bytes = write(sockets, m, sizeof(m));

    //если передать получилось, то возвращаем true, иначе - false
    return bytes > 0;
}

//отправить сообщение
bool TcpClient::send(const size_t n) {
    bzero(m, sizeof(m));
    std::string temp = std::to_string(n);
    strcpy(m, temp.c_str());
    ssize_t bytes = write(sockets, m, sizeof(m));

    //если передать получилось, то возвращаем true, иначе - false
    return bytes > 0;
}

//получить сообщение
void TcpClient::receive(char* buf, short length) {
    bzero(m, sizeof(m));
    bzero(buf, length);
    read(sockets, m, sizeof(m));
    strcpy(buf, m);
}

//завершить соединение
void TcpClient::disconnect() {
    close(sockets);
}