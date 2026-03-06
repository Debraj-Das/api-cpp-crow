#pragma once
#include "../headers/crow_all.h"
#include <string>

// 1. Inherit from ILocalMiddleware so it doesn't run globally
struct AdminAuth : crow::ILocalMiddleware {
    
    // 2. Define the "backpack" (context) for this middleware
    struct context {
        int user_id;
        std::string role;
    };

    void before_handle(crow::request& req, crow::response& res, context& ctx) {
        std::string auth_header = req.get_header_value("Authorization");
        
        // Security Check
        if (auth_header != "SecretAdminToken") {
            res.code = 401;
            res.body = "Unauthorized Access!";
            res.end(); // Stop the request here, it never reaches the controller
            return;
        }

        // If successful, store data in the context backpack!
        ctx.user_id = 99; 
        ctx.role = "Administrator";
    }

    void after_handle(crow::request& req, crow::response& res, context& ctx) {
        // You can leave this empty if you don't need to do anything after
    }
};
