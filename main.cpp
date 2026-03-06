#include "headers/crow_all.h"
#include "middlewares/auth_middleware.h"
#include "middlewares/logger.h"
#include "repositories/userRepository.h"
#include "routers/userRouter.h"

// 1. Initialize App with Middlewares, else use SimpleApp
using Apptype = crow::App<LoggerMiddleware, AdminAuth>;
constexpr int PORT = 8080;

int main() {
    Apptype app;

    // 2. Initialize Database
    UserRepository::init_database();

    // 3. Register Routers
    UserRouter::register_routes<Apptype>(app);

    // If you had a billing router, you would just add:
    // BillingRouter::register_routes(app);

    // 4. Start Server
    app.port(PORT).multithreaded().run();
    return 0;
}
