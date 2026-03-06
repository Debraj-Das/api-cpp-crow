#pragma once
#include "../repositories/userRepository.h"
#include <string>
#include <vector>

namespace UserService {

inline bool addUser(const std::string &username, const std::string &age) {
    if (username.empty() || age.empty())
        return false; // Basic validation
    UserRepository::User newUser = {0, username, age};
    return UserRepository::create_user(newUser);
}

inline std::vector<UserRepository::User> retrieveAllUsers() {
    return UserRepository::get_all_users();
}

inline bool modifyUser(int id, const std::string &username,
                       const std::string &age) {
    UserRepository::User updatedUser = {id, username, age};
    return UserRepository::update_user(id, updatedUser);
}

inline bool removeUser(int id) { return UserRepository::delete_user(id); }

inline std::optional<UserRepository::User> retrieveOneUser(int id) {
    return UserRepository::get_user_by_id(id);
}
} // namespace UserService
