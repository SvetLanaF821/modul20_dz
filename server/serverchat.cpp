#include "serverchat.h"
#include <iostream>
#include <memory>
#include <exception>
#include <limits>

//установить цвет текста
void ServerChat::setColor(const std::string& color) const {
    if (color == "lightBlue") {                         //голубой
        std::cout << "\033[01;38;05;51m";
    }
    else if (color == "blue") {                         //синий
        std::cout << "\033[01;38;05;33m";
    }
    else if (color == "yellow") {                       //желтый
        std::cout << "\033[01;38;05;178m";
    }
    else if (color == "green") {                        //зеленый
        std::cout << "\033[01;38;05;120m";
    }
    else if (color == "red") {                          //красный
        std::cout << "\033[01;38;05;196m";
    }
    else if (color == "default") {                      //сброс цветов
        std::cout << "\033[0m";
    }
}

//работа чата
int ServerChat::working() {
    try {
        server_->config();                              //настроить сервер
        char m[lengthMessage]{};                        //массив для получения сообщений
        bzero(m, lengthMessage);
        bool quit = true;

        while (quit) {
            server_->listenAndWait();                   //сервер ждет подключения
          
            while (strncmp("END", m, 3) != 0) {
                server_->receive(m, lengthMessage);     //получить данные
                //если пришла команда для выхода из программы
                if (strncmp("EXT", m, 3) == 0) {
                    quit = false;
                    break;
                }
                //если пришла команда для входа в чат
                if (strncmp("ENT", m, 3) == 0) {
                    std::cout << "Запрос на вход получен" << std::endl;

                    server_->receive(m, lengthMessage); //принять логин
                    std::string login(m);

                    server_->receive(m, lengthMessage); //принять пароль
                    std::string password(m);

                    authorizationUser(login, password);
                    bzero(m, lengthMessage);
                }

                //если пришла команда для получения сообщений
                else if (strncmp("MSG", m, 3) == 0) {
                    std::cout << "Запрос на получение сообщений получен" << std::endl;
                    
                    server_->receive(m, lengthMessage); //принять логин
                    std::string login(m);

                    server_->receive(m, lengthMessage); //принять сообщение
                    size_t amount = static_cast<size_t>(atoi(m));

                    sendMessages(login, amount);
                    bzero(m, lengthMessage);
                }

                //если пришла команда для регистрации пользователя
                else if (strncmp("USR", m, 3) == 0) {
                    std::cout << "Запрос на добавление пользователя получен" << std::endl;

                    server_->receive(m, lengthMessage); //принять логин
                    std::string login(m);

                    server_->receive(m, lengthMessage); //принять пароль
                    std::string password(m);

                    int res = addUser(login, password);

                    if (res == 1) {
                        setColor("green");
                        std::cout << "Пользователь " << login << " добавлен" << std::endl;
                        setColor("default");
                    }
                    else if (res == 2) {
                        setColor("red");
                        std::cout << "Пользователь " << login << " не добавлен" << std::endl;
                        setColor("default");
                    }

                    bzero(m, lengthMessage);
                }

                //если пришла команда для добавления сообщения
                else if (strncmp("ADM", m, 3) == 0) {
                    std::cout << "Запрос на отправку сообщения получен" << std::endl;

                    server_->receive(m, lengthMessage); //принять логин отправителя
                    std::string from(m);

                    
                    server_->receive(m, lengthMessage); //принять логин получателя
                    std::string to(m);

                    server_->receive(m, lengthMessage); //принять текст сообщения
                    std::string text(m);

                    int res = addMessage(from, to, text);

                    if (res == 1) {
                        setColor("green");
                        std::cout << "Пользователь " << from << " отправил сообщение" << std::endl;
                        setColor("default");
                    }
                    else if (res == 2) {
                        setColor("red");
                        std::cout << "Пользователь " << from << " не отправил сообщение " << std::endl;
                        setColor("default");
                    }

                    bzero(m, lengthMessage);
                }

                //если пришла команда для выхода пользователя
                else if (strncmp("OUT", m, 3) == 0) {
                    server_->receive(m, lengthMessage);
                    std::string login(m);
                    outUser(login);
                    bzero(m, lengthMessage);
                }
            }
            bzero(m, lengthMessage);
        }
        server_->stop();                               //закрыть соединение
    }
    catch (const std::string& e) {
        std::cout << e << std::endl;
        return 1;
    }
    catch (const char* e) {
        std::cout << e << std::endl;
        return 1;
    }
    catch (...) {
        setColor("red");
        std::cout << "Ошибка. Неизвестное исключение." << std::endl;
        setColor("default");
        return 1;
    }

    return 0;
}

//авторизация пользователя
bool ServerChat::authorizationUser(const std::string& login, const std::string& password) {
    for (auto& user : users_) {
        if (user.checkLogin(login)) {                   //если логин есть
            if (user.checkPassword(password)) {         //если пароль есть
                user.setAuthUser();
                server_->send(true);
                setColor("blue");
                std::cout << "Пользователь " << login << " успешно авторизовался" << std::endl;
                setColor("default");
                return true;
            }
        }
    }

    //если логина/пароля нет
    server_->send(false);
    setColor("red");
    std::cout << "пользователь " << login << " не авторизовался" << std::endl;
    setColor("default");
    return false;
}

//отправить сообщения на клиент
void ServerChat::sendMessages(const std::string& login, const size_t amount) {
    std::vector<Message> v;                             //вектор для найденных сообщений

    for (const auto& mess : messages_) {
        if (mess.toThisLogin(login) || mess.toThisLogin("all")) {
            v.push_back(mess);                          //добавить сообщения в вектор
        }
    }

    //если количество сообщений на клиенте меньше, чем на сервере   
    if (amount < v.size()) {
        size_t diff = v.size() - amount;
        server_->send(diff);

        for (size_t i = amount; i < v.size(); ++i) {
            //отправить данные сообщения
            server_->send((v[i].getFrom()).c_str());
            server_->send((v[i].getTo()).c_str());
            server_->send((v[i].getText()).c_str());
        }
        return;
    }
    else {
        server_->send(0);
    }
}

//выход пользователя
void ServerChat::outUser(const std::string& login) {
    for (auto &user : users_) {
        if (user.checkLogin(login)) {
            user.resetAuthUser();
            setColor("blue");
            std::cout << "Пользователь " << login << " вышел из системы" << std::endl;
            setColor("default");
            return;
        }
    }

    setColor("red");
    std::cout << "Пользователь " << login << " не смог выйти из системы. Он не найден." << std::endl;
    setColor("default");
}

//добавить пользователя
int ServerChat::addUser(const std::string& login, const std::string& password) {
    for (const auto& user : users_) {
        if (user.checkLogin(login)) {                   //есть ли логин
            server_->send(2);
            return 2;
        }
    }
  
    users_.emplace_back(login, password);               //добавить пользователя
    server_->send(1);

    //добавлить сообщение для нового пользователя от админа
    messages_.emplace_back("admin", login, "Добро пожаловать в чат.");
    return 1;
}

//добавить сообщение
int ServerChat::addMessage(const std::string& from, const std::string& to, const std::string& text) {
    bool bit = true;
    if (to != "all") {
        bit = false;
        for (const auto& user : users_) {
            if (user.checkLogin(to)) {                  //есть ли получатель
                std::cout << "Логин есть " << to << std::endl;
                bit = true;
                break;
            }
        }
    }

    if (!bit) {
        server_->send(2);
        return 2;
    }

    messages_.emplace_back(from, to, text);             //добавить сообщение
    server_->send(1);                                   //отправить сигнал успешного добавления

    return 1;
}