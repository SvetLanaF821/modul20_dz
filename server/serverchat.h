#pragma once

#include "iserver.h"
#include "tcpserver.h"
#include "user.h"
#include "message.h"
#include "sha256.h"
#include <memory>
#include <vector>

class ServerChat {
    std::unique_ptr<IServer> server_;                   //указатель для работы с сервером
    std::vector<User> users_;                           //вектор пользователей
    std::vector<Message> messages_;                     //вектор сообщений

public:
    ServerChat() : server_(new TcpServer(55555)), users_(), messages_() {
        SHA256 sha256;
        users_.emplace_back("admin", sha256("a123"));   //добавляем админа
    }
    ~ServerChat() = default;

    void setColor(const std::string& color) const;      //установить цвет текста
    int working();                                      //работа чата

    //авторизация пользователя
    bool authorizationUser(const std::string& login, const std::string& password);

    //отправить сообщения на клиент
    void sendMessages(const std::string& login, const size_t amount);

    //выход пользователя
    void outUser(const std::string& login);

    //добавить пользователя
    int addUser(const std::string& login, const std::string& password);

    //добавить сообщение
    int addMessage(const std::string& from, const std::string& to, const std::string& text);
};