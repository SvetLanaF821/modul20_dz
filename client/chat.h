#pragma once

#include "message.h"
#include "sha256.h"
#include "tcpclient.h"
#include "iclient.h"
#include <memory>
#include <vector>

class Chat {
	std::string login_;										//логин
	std::string ip_;										//IP-адрес
	uint16_t port_;											//порт
	std::vector<Message> messages_;							//вектор сообщений
	std::unique_ptr<IClient> client_;						//клиент

public:
	Chat() : login_(), ip_("127.0.0.1"), port_(55555), messages_(), client_(new TcpClient) {}
	virtual ~Chat() = default;

	void setColor(const std::string& color) const;			//установить цвет текста в терминале
	void working();											//работа чата
	bool authorizationUser(std::string login, std::string password);		//авторизация пользователя
	void receiveMessages(const std::string& login);			//получить сообщения от сервера
	void showAllChat();										//показать общий чат
	void showSelfChat();									//показать чат с личными сообщениями
	void showUserMenu();									//показать меню пользователя
	int calculateAllMessages() const;						//подсчитать количество общих сообщений
	int calculateSelfMessages() const;						//подсчитать количество личных сообщений
	void outUser();											//запрос на выход пользователя
	void writeMessage();									//написать сообщения
	void userRegistration();								//регистрация пользователя
	void chatEntry();										//вход в чат
};