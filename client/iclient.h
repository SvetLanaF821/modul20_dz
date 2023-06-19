#pragma once

#include <arpa/inet.h>

class IClient {
public:
	virtual ~IClient() = default;
	virtual void config(const char* address, uint16_t port) = 0;	//настроить клиент
	virtual void connectToServer() = 0;								//соединение с сервером
	virtual bool send(const char* message) = 0;						//отправить сообщение
	virtual bool send(const int n) = 0;								//отправить сообщение
	virtual bool send(const unsigned n) = 0;						//отправить сообщение
	virtual bool send(const bool bit) = 0;							//отправить сообщение
	virtual bool send(const size_t n) = 0;							//отправить сообщение
	virtual void receive(char* buf, short length) = 0;				//получить сообщение
	virtual void disconnect() = 0;									//завершить соединение
};