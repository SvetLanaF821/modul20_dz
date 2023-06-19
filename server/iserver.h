#pragma once

#include <cstddef>

class IServer {
public:
	virtual ~IServer() = default;
	virtual void config() = 0;							//настройка сервера
	virtual void listenAndWait() = 0;					//слушать и ждать соединения
	virtual void stop() = 0;							//остановка сервера
	virtual void receive(char* buf, short length) = 0;	//получить сообщения от клиента
	virtual bool send(const char* message) = 0;			//отправка сообщения клиенту
	virtual bool send(const int n) = 0;					//отправка сообщения клиенту
	virtual bool send(const size_t n) = 0;				//отправка сообщения клиенту
	virtual bool send(const unsigned n) = 0;			//отправка сообщения клиенту
	virtual bool send(const bool bit) = 0;				//отправка сообщения клиенту
};