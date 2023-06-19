#include "chat.h"
#include <iostream>
#include <cstdlib>
#include <algorithm> 
#include <limits>
#include <stdio.h>

//установить цвет текста в терминале
void Chat::setColor(const std::string& color) const {
    if (color == "lightBlue") {                             //голубой
        std::cout << "\033[01;38;05;51m";
    }
    else if (color == "blue") {                             //синий
        std::cout << "\033[01;38;05;33m";
    }
    else if (color == "yellow") {                           //желтый
        std::cout << "\033[01;38;05;178m";
    }
    else if (color == "green") {                            //зеленый
        std::cout << "\033[01;38;05;120m";
    }
    else if (color == "red") {                              //красный
        std::cout << "\033[01;38;05;196m";
    }
    else if (color == "default") {                          //сброс цветов
        std::cout << "\033[0m";
    }
}

//работа чата
void Chat::working()                                        
{
    char choice;
    bool b{ true };
    while (b) {
        setColor("lightBlue");
        std::cout << "\n********* Главное меню **********" << std::endl;
        std::cout << "q - Выход " << std::endl;
        std::cout << "1 - Регистрация нового пользователя " << std::endl;
        std::cout << "2 - Вход в чат " << std::endl;
        setColor("default");
        std::cin >> choice;
        switch (choice) {
        case 'q':
            b = false;
            client_->config(ip_.c_str(), port_);            //настраить клиент
            client_->connectToServer();                     //подлючиться к серверу
            client_->send("EXT");
            client_->disconnect();                          //отключиться от сервера
            break;
        case '1':
            userRegistration();
            break;
        case '2':
            chatEntry();
            break;
        default:
            setColor("red");
            std::cout << "Повторите, пожалуйста..." << std::endl;
            break;
        }
    }
}

//авторизация пользователя
bool Chat::authorizationUser(std::string login, std::string password) {
    client_->send("ENT");
    client_->send(login.c_str());
    client_->send(password.c_str());

    char m[lengthMessage];
    client_->receive(m, lengthMessage);

    return bool(atoi(m));
}

//получить сообщения от сервера
void Chat::receiveMessages(const std::string& login) {
    client_->send("MSG");
    client_->send(login.c_str());
    client_->send(messages_.size());

    char m[lengthMessage];
    client_->receive(m, lengthMessage);

    size_t amount = static_cast<size_t>(atoi(m));

    //получить сообщение
    for (size_t i = 0; i < amount; ++i) {
        //получить информацию об отправителе
        client_->receive(m, lengthMessage);
        std::string from = m;

        //получить информацию о получателе
        client_->receive(m, lengthMessage);
        std::string to = m;

        //получить сообщение
        client_->receive(m, lengthMessage);
        std::string text = m;

        messages_.emplace_back(from, to, text);
    }
}

//показать общий чат
void Chat::showAllChat() {
    setColor("green");
    std::cout << "\n********** Общий чат **********" << std::endl;
    //есть ли сообщения в общем чате
    if (calculateAllMessages() == 0) {
        std::cout << std::endl << "Сообщений нет" << std::endl;
    }
    else {
        for (const auto& message : messages_) {
            //если поле "кому" содержит all
            if (message.toThisLogin("all")) {
                //вывести отправителя и текст сообщения
                std::cout << "Отправитель: " << message.getFrom() << std::endl;
                std::cout << message.getText() << std::endl;
                std::cout << std::endl;
            }
        }
    }
    std::cout << "---------------- конец ------------------" << std::endl;
    setColor("default");
}

//показать чат с личными сообщениями
void Chat::showSelfChat() {
    setColor("yellow");
    std::cout << "\n********** Личные сообщения **********" << std::endl;
    //если количество личных сообщений равно 0
    if (calculateSelfMessages() == 0) {
        std::cout << std::endl << "Сообщений нет" << std::endl;
    }
    else {
        for (const auto& message : messages_) {
            //если поле "кому" соответствует логину текущего пользователя
            if (message.toThisLogin(login_)) {
                //Выводим отправителя и сообщение
                std::cout << "Отправитель: " << message.getFrom() << std::endl;
                std::cout << message.getText() << std::endl;
                std::cout << std::endl;
            }
        }
    }
    std::cout << "---------------- конец ------------------" << std::endl;
    setColor("default");
}

//показать меню пользователя
void Chat::showUserMenu() {
    char choice = '0';                                  //выбор в чате

    while (true) {
        setColor("blue");
        receiveMessages(login_);
        std::cout << "\n********** Меню пользователя **********" << std::endl;
        std::cout << "\nq - Выход \n1 - Общий чат \n2 - Личные сообщения \n3 - Написать сообщение " << std::endl;
        setColor("default");
        std::cout << "\nВведите символ: ";
        std::cin >> choice;

        if (choice == 'q') {
            break;
        }
        else if (choice == '1') {
            showAllChat();

            continue;
        }
        else if (choice == '2') {
            showSelfChat();
            continue;
        }
        else if (choice == '3') {
            writeMessage();
        }
    }
}

//подсчитать количество общих сообщений
int Chat::calculateAllMessages() const {
    int amount = std::count_if(messages_.begin(), messages_.end(), [](const Message& m) {
        return m.toThisLogin("all");
        });

    return amount;
}

//подсчитать количество личных сообщений
int Chat::calculateSelfMessages() const {
    std::string login = login_;
    //Подсчитываем количество совпадений
    int amount = std::count_if(messages_.begin(), messages_.end(), [login](const Message& m) {
        return m.toThisLogin(login);
        });
    return amount;
}

//запрос на выход пользователя
void Chat::outUser() {
    client_->send("OUT");
    client_->send(login_.c_str());
    login_.clear();
    messages_.clear();
}

//написать сообщения
void Chat::writeMessage() {
    std::string to;
    std::string text;

    std::cout << "Получатель (all или логин пользователя): ";
    std::cin >> to;
    std::cout << "Текст сообщения: ";
    std::cin.ignore();
    std::getline(std::cin, text);

    //если поле пустое
    if (text.empty()) {
        setColor("red");
        std::cout << "Ошибка. Сообщение пустое." << std::endl;
        setColor("default");
        return;
    }

    //отправить данные на сервер
    client_->send("ADM");
    client_->send(login_.c_str());
    client_->send(to.c_str());
    client_->send(text.c_str());

    //получить результат
    char m[lengthMessage];
    client_->receive(m, lengthMessage);

    int res = atoi(m);

    if (res == 1) {
        std::cout << "Сообщение отправлено!" << std::endl;
    }
    else if (res == 2) {
        setColor("red");
        std::cout << "Ошибка. Пользователя с таким логином нет." << std::endl;
        setColor("default");
    }
}

//регистрация пользователя
void Chat::userRegistration() {
    std::string tlogin;
    std::string tpassword;

    try {
        client_->config(ip_.c_str(), port_);            //настроить клиент
        client_->connectToServer();                     //подлючиться к серверу
    }
    catch (const char* e) {
        setColor("red");
        std::cout << e << std::endl;
        setColor("default");
        return;
    }
    catch (...) {
        setColor("red");
        std::cout << "Ошибка. Неизвестное исключение!" << std::endl;
        setColor("default");
        return;
    }

    std::cout << "Введите логин: ";
    std::cin >> tlogin;

    //если длина логина меньше 3 
    if (tlogin.length() < 3) {
        setColor("red");
        std::cout << "Ошибка. Логин должен быть более 2 символов." << std::endl;
        setColor("default");
        client_->send("END");
        client_->disconnect();
        return;
    }
    //если длина логина больше 8
    if (tlogin.length() > 8) {
        setColor("red");
        std::cout << "Ошибка. Логин должен быть не более 8 символов." << std::endl;
        setColor("default");
        client_->send("END");
        client_->disconnect();
        return;
    }

    std::cout << "Пароль: ";
    std::cin >> tpassword;

    SHA256 sha256;

    //отправить логин и пароль
    client_->send("USR");
    client_->send(tlogin.c_str());
    client_->send((sha256(tpassword)).c_str());

    //получить результат
    char m[lengthMessage];
    client_->receive(m, lengthMessage);

    int res = atoi(m);

    if (res == 1) {
        std::cout << "Вы зарегистрированны." << std::endl;
    }
    else if (res == 2) {
        setColor("red");
        std::cout << "Ошибка. Такой логин уже есть!" << std::endl;
        setColor("default");
    }

    client_->send("END");
    client_->disconnect();
}

//вход в чат
void Chat::chatEntry() {
    std::string tlogin;
    std::string tpassword;
    std::cout << "Логин: ";
    std::cin >> tlogin;
    std::cout << "Пароль: ";
    std::cin >> tpassword;

    
    try {
        client_->config(ip_.c_str(), port_);        //настроить клиент
        client_->connectToServer();                 //подключиться к серверу
    }
    catch (const char* e) {
        setColor("red");
        std::cout << e << std::endl;
        setColor("default");
        return;
    }
    catch (...) {
        setColor("red");
        std::cout << "Неизвестное исключение!" << std::endl;
        setColor("default");
        return;
    }

    SHA256 sha256;                              //создаем объект для хеширования пароля
    bool res = authorizationUser(tlogin, sha256(tpassword));

    //если пользователь не найден
    if (!res) {
        setColor("red");
        std::cout << "Ошибка. Логин/пароль введен неверно!" << std::endl;
        setColor("default");
        client_->send("END");
        client_->disconnect();
        return;
    }
    login_ = tlogin;
    messages_.clear();

    showUserMenu();                             //показать меню пользователя
    outUser();                                  //отправить запрос на выход из системы

    client_->send("END");
    client_->disconnect();
}