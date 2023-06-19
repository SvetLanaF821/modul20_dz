#include "user.h"

//получить логин
const std::string& User::getLogin() const {
	return _login;
}

//проверить логин
bool User::checkLogin(const std::string& login) const {
	return _login == login;
}

//проверить пароль
bool User::checkPassword(const std::string& password) const {
	return _password == password;
}

//установить статус авторизации
void User::setAuthUser() {
	_authUser = true;
}

//сбросить статус авторизации
void User::resetAuthUser() {
	_authUser = false;
}

//получить статус авторизации
bool User::getAuthUser() const {
	return _authUser;
}