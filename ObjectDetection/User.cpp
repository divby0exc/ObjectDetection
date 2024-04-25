#include "User.h"

User::User(const std::string& username_t, const std::string& password_t) : username(username_t), password(password_t) {}
User::User() {}
std::string User::get_username() const { return username; }
std::string User::get_password() const { return password; }
void User::set_username(std::string&& username_t) { username = username_t; }
void User::set_password(std::string&& password_t) { password = password_t; }
User::~User() {};