#include "tcpserver.h"
#include <iostream>
#include <string>

TcpServer::TcpServer() : sockets_(-1), serverAddr_(), client_(), port_(55555), connection_(-1) {
    bzero(m_, lengthMessage);
}

TcpServer::TcpServer(uint16_t port) : sockets_(-1), serverAddr_(), client_(), port_(port), connection_(-1) {
    bzero(m_, lengthMessage);
}

//настроить сервер
void TcpServer::config() {
    std::cout << "Настройка сервера начата." << std::endl;
    sockets_ = socket(AF_INET, SOCK_STREAM, 0);         //создать сокет

    if (sockets_ == -1) {
        std::string str = "Исключение: не удалось создать сокет!";
        throw str;
    }

    serverAddr_.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr_.sin_port = htons(port_);                //задать номер порта
    serverAddr_.sin_family = AF_INET;                   //использовать IPv4

    //привязать сокет
    int bind_status = bind(sockets_, (struct sockaddr*)&serverAddr_, sizeof(serverAddr_));

    //если привязать не удалось
    if (bind_status == -1) {
        std::string str = "Исключение: не удалось создать сокет!";
        throw str;
    }

    std::cout << "Сервер использует порт: " << port_ << std::endl;
    std::cout << "Настройка сервера закончена." << std::endl;
}

//слушать и ждать соединение
void TcpServer::listenAndWait() {
   
    int connection_status = listen(sockets_, 5);         //сервер слушает соединение

    if (connection_status == -1) {
        std::string str = "Исключение: сервер не может прослушивать новые соединения.";
        throw str;
    }
    else {
        std::cout << "Сервер ожидает новых подключений." << std::endl;
    }

    socklen_t length = sizeof(client_);
    connection_ = accept(sockets_, (struct sockaddr*)&client_, &length);

    if (connection_ == -1) {
        std::string str = "Исключение: сервер не может принять данные от клиента.";
        throw str;
    }

    std::cout << "Клиент подключился." << std::endl;
}

//остановить соединение
void TcpServer::stop() {
    close(sockets_);
    std::cout << "Сервер остановлен." << std::endl;
}

//принять сообщение от клиента
void TcpServer::receive(char* buf, short length) {
    bzero(m_, sizeof(m_));
    bzero(buf, length);

    ssize_t bytes = read(connection_, m_, sizeof(m_));
    if (bytes > 0) {
        strcpy(buf, m_);
    }
    else {
        std::cout << "Ошибка получения данных от клиента." << std::endl;
    }
}

//отправить сообщение клиенту
bool TcpServer::send(const char* message) {
    bzero(m_, sizeof(m_));
    strcpy(m_, message);
    ssize_t bytes = write(connection_, m_, sizeof(m_));

    //если передать получилось, то возвращаем true, иначе - false
    return bytes > 0;
}

//отправить сообщение клиенту
bool TcpServer::send(const int n) {
    bzero(m_, sizeof(m_));
    std::string temp = std::to_string(n);

    strcpy(m_, temp.c_str());

    ssize_t bytes = write(connection_, m_, sizeof(m_));
    //если передать получилось, то возвращаем true, иначе - false
    return bytes > 0;
}

//отправить сообщение клиенту
bool TcpServer::send(const size_t n) {
    bzero(m_, sizeof(m_));
    std::string temp = std::to_string(n);

    strcpy(m_, temp.c_str());

    ssize_t bytes = write(connection_, m_, sizeof(m_));
    //если передать получилось, то возвращаем true, иначе - false
    return bytes > 0;
}

//отправить сообщение клиенту
bool  TcpServer::send(const unsigned n) {
    bzero(m_, sizeof(m_));

    std::string temp = std::to_string(n);

    strcpy(m_, temp.c_str());

    ssize_t bytes = write(connection_, m_, sizeof(m_));
    //если передать получилось, то возвращаем true, иначе - false
    return bytes > 0;
}

//отправить сообщение клиенту
bool TcpServer::send(const bool bit) {
    bzero(m_, sizeof(m_));

    if (bit) {
        strcpy(m_, "1");
    }
    else {
        strcpy(m_, "0");
    }

    ssize_t bytes = write(connection_, m_, sizeof(m_));
    //если передать получилось, то возвращаем true, иначе - false
    return bytes > 0;
}