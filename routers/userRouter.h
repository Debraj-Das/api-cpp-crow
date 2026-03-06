#pragma once
#include "../headers/crow_all.h"
#include "../controllers/userController.h"
#include "../middlewares/auth_middleware.h"

namespace UserRouter {

    template <typename AppType>
    inline void register_routes(AppType& app) {
        
        // 1. Use 'new' to allocate blueprints on the heap.
        // This guarantees they survive after the function finishes!
        auto* bp_public = new crow::Blueprint("api/v1/users", "");
        auto* bp_protected = new crow::Blueprint("api/v1/users", ""); // not build the static variable use "" after the path

        // 2. Dereference the pointers when applying middleware
        bp_protected->CROW_MIDDLEWARES(app, AdminAuth); // we can add multiple middleware options sequencely one after AdminAuth, as (app, AdminAuth, somemiddle, some2middle, ...)

        // 3. Dereference the pointers (*bp_public) inside the routing macros
        CROW_BP_ROUTE((*bp_public), "")
            .methods("GET"_method)(UserController::getAllUsers);
            
        CROW_BP_ROUTE((*bp_public), "/<int>")
            .methods("GET"_method)(UserController::getOneUser);

        CROW_BP_ROUTE((*bp_protected), "")
            .methods("POST"_method)(UserController::createUser);
            
        CROW_BP_ROUTE((*bp_protected), "/<int>")
            .methods("PUT"_method)(UserController::updateUser);
            
        CROW_BP_ROUTE((*bp_protected), "/<int>")
            .methods("DELETE"_method)(UserController::deleteUser);

        // 4. Register the dereferenced pointers to the app
        app.register_blueprint(*bp_public);
        app.register_blueprint(*bp_protected);
    }
}
