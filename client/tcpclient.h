#pragma once

#include "iclient.h"
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>

constexpr short lengthMessage = 1024;

class TcpClient : public IClient {
	int sockets;											//сокет
	struct sockaddr_in serveraddr;							//адрес сервера
	struct sockaddr_in client;								//адрес клиента
	char m[lengthMessage];									//массив для сообщения

public:
	TcpClient();
	virtual ~TcpClient() override = default; 

	void config(const char* address, uint16_t port) override;//настроить клиент
	void connectToServer() override;						//соединение с сервером
	bool send(const char* message) override;				//отправить сообщение
	bool send(const int n) override;						//отправить сообщение
	bool send(const unsigned n) override;					//отправить сообщение
	bool send(const bool bit) override;						//отправить сообщение
	bool send(const size_t n) override;						//отправить сообщение
	void receive(char* buf, short length) override;			//получить сообщение
	void disconnect() override;								//завершить соединение
};