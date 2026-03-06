#include "controllers/userController.h"
#include "header/crow_all.h"
#include "repositories/userRepository.h"

int main() {
    crow::SimpleApp app;

    // Initialize SQLite Database on startup
    UserRepository::init_database();

    // Map the CRUD Routes
    CROW_ROUTE(app, "/users")
        .methods("POST"_method)(UserController::createUser); // CREATE
    CROW_ROUTE(app, "/users")
        .methods("GET"_method)(UserController::getAllUsers); // READ
    CROW_ROUTE(app, "/users/<int>")
        .methods("PUT"_method)(UserController::updateUser); // UPDATE
    CROW_ROUTE(app, "/users/<int>")
        .methods("DELETE"_method)(UserController::deleteUser); // DELETE
    CROW_ROUTE(app, "/users/<int>")
        .methods("GET"_method)(UserController::getOneUser);

    app.port(8080).multithreaded().run();
    return 0;
}
