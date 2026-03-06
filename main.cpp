#include "controllers/userController.h"
#include "headers/crow_all.h"
#include "repositories/userRepository.h"
#include "middlewares/logger.h"
#include "middlewares/auth_middleware.h"

int main() {
    crow::App<LoggerMiddleware, AdminAuth> app;

    // Initialize SQLite Database on startup
    UserRepository::init_database();

    // Map the CRUD Routes
    CROW_ROUTE(app, "/users")
        .methods("POST"_method).CROW_MIDDLEWARES(app, AdminAuth)(UserController::createUser); // CREATE
    CROW_ROUTE(app, "/users")
        .methods("GET"_method)(UserController::getAllUsers); // READ
    CROW_ROUTE(app, "/users/<int>")
        .methods("PUT"_method).CROW_MIDDLEWARES(app, AdminAuth)(UserController::updateUser); // UPDATE
    CROW_ROUTE(app, "/users/<int>")
        .methods("DELETE"_method).CROW_MIDDLEWARES(app, AdminAuth)(UserController::deleteUser); // DELETE
    CROW_ROUTE(app, "/users/<int>")
        .methods("GET"_method)(UserController::getOneUser);

    app.port(8080).multithreaded().run();
    return 0;
}
