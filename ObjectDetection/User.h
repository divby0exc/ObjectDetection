#pragma once
#include <string>

class User {
	std::string username;
	std::string password;

public:
	User(const std::string& username_t, const std::string& password_t);
	User();
	std::string get_username() const;
	std::string get_password() const;
	void set_password(std::string&& username);
	void set_username(std::string&& password);
	~User();
};