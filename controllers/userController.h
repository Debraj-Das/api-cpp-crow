#pragma once
#include "../headers/crow_all.h"
#include "../services/userService.h"

namespace UserController {

inline crow::response createUser(const crow::request &req) {
    auto body = crow::json::load(req.body);
    if (!body)
        return crow::response(400, "Invalid JSON");

    std::string username = body["username"].s();
    std::string age = body["age"].s();

    if (UserService::addUser(username, age)) {
        return crow::response(201, "User created successfully");
    }
    return crow::response(500, "Failed to create user");
}

inline crow::response getAllUsers() {
    std::vector<UserRepository::User> users = UserService::retrieveAllUsers();

    // Convert C++ Vector of Users into a JSON array
    std::vector<crow::json::wvalue> json_users;
    for (const auto &u : users) {
        crow::json::wvalue json_u;
        json_u["id"] = u.id;
        json_u["username"] = u.username;
        json_u["age"] = u.age;
        json_users.push_back(std::move(json_u));
    }

    return crow::response(200, crow::json::wvalue(json_users));
}

inline crow::response updateUser(const crow::request &req, int id) {
    auto body = crow::json::load(req.body);
    if (!body)
        return crow::response(400, "Invalid JSON");

    std::string username = body["username"].s();
    std::string age = body["age"].s();

    if (UserService::modifyUser(id, username, age)) {
        return crow::response(200, "User updated successfully");
    }
    return crow::response(500, "Failed to update user");
}

inline crow::response deleteUser(int id) {
    if (UserService::removeUser(id)) {
        return crow::response(200, "User deleted successfully");
    }
    return crow::response(500, "Failed to delete user");
}

// ... inside namespace UserController ...

inline crow::response getOneUser(int id) {
    std::optional<UserRepository::User> user_opt = UserService::retrieveOneUser(id);
    
    // If the user does not exist (std::nullopt)
    if (!user_opt.has_value()) {
        return crow::response(404, "User not found");
    }

    // If the user exists, extract them from the optional
	UserRepository::User u = user_opt.value();

    // Build the JSON response
    crow::json::wvalue json_u;
    json_u["id"] = u.id;
    json_u["username"] = u.username;
    json_u["age"] = u.age;

    return crow::response(200, json_u);
}
} // namespace UserController
