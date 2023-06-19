#include "message.h"

Message::Message() : _from("UNKNOWN"), _to("UNKNOWN"), _text("UNKNOWN") {}

Message::Message(const std::string& from, const std::string& to, const std::string& text) : _from(from), _to(to), _text(text) {}

Message::Message(const Message& other) : _from(other._from), _to(other._to), _text(other._text) {}

const std::string& Message::getFrom() const {
	return _from;
}

const std::string& Message::getTo() const {
	return _to;
}

const std::string& Message::getText() const {
	return _text;
}

bool Message::toThisLogin(const std::string& login) const {
	return _to == login;
}