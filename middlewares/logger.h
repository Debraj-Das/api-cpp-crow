#pragma once
#include "../headers/crow_all.h"
#include <iostream>
#include <chrono>

struct LoggerMiddleware {
    // Context to pass data between before_handle and after_handle
    struct context {
        std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
    };

    void before_handle(crow::request& req, crow::response& res, context& ctx) {
        // Record the exact microsecond the request arrives
        ctx.start_time = std::chrono::high_resolution_clock::now();
        std::cout << "[INFO] Incoming Request: " << crow::method_name(req.method)<< " " << req.url << "\n";
    }

    void after_handle(crow::request& req, crow::response& res, context& ctx) {
        // Calculate how long your C++ code took to process the request
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - ctx.start_time);
        
        std::cout << "[INFO] Responded with status " << res.code 
                  << " in " << duration.count() << "ms\n";
    }
};
