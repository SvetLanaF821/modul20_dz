#pragma once

#include "iserver.h"
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

constexpr short lengthMessage = 1024;					//длина сообщения

class TcpServer : public IServer {
	int sockets_;										//сокет для обмена данными
	struct sockaddr_in serverAddr_;						//адрес сервера
	struct sockaddr_in  client_;						//адрес клиента
	uint16_t port_;										//порт  
	char m_[lengthMessage];								//массив для отправки и получения сообщений
	int connection_;									//соединение

public:
	TcpServer();
	explicit TcpServer(uint16_t port);
	virtual ~TcpServer() override = default;

	void config() override;								//настроить сервер
	void listenAndWait() override;						//слушать и ждать соединение
	void stop() override;								//остановить соединение
	void receive(char* buf, short length) override;		//принять сообщение от клиента
	bool send(const char* message) override;			//отправить сообщение клиенту
	bool send(const int n) override;					//отправить сообщение клиенту
	bool send(const size_t n) override;					//отправить сообщение клиенту
	bool send(const unsigned n) override;				//отправить сообщение клиенту
	bool send(const bool bit) override;					//отправить сообщение клиенту
};