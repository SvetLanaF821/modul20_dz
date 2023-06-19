#pragma once

#include <string>

class Message {
	const std::string _from;							//от кого 
	const std::string _to;								//кому 
	const std::string _text;							//текст сообщения

public:
	Message();
	Message(const std::string& from, const std::string& to, const std::string& text);
	Message(const Message& other);
	~Message() = default;

	const std::string& getFrom() const;
	const std::string& getTo() const;
	const std::string& getText() const;
	bool toThisLogin(const std::string& login) const;
};