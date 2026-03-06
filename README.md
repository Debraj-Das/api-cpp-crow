# Back-end Development using C++ CROW

Setting up a CRUD API with Crow on a Linux system is remarkable straight forward. Because Crow is a "header-only" framework, you do not have to go through complex installation processes or build massive libraries; you mostly just need the header file and a standard system dependency.

**`./learning/` repository for learning some extra topic related to api development**

- [official github](https://github.com/CrowCpp/Crow)

Here is the step-by-step guideline to get your first C++ API runing from scratch.

## Step 1: Install System dependencies

- Crow relies on `asio` for handling asynchronous network connections. Open your Linux terminal and install a C++ compiler (`build-essential`) and the standalone `asio` development library.

```sh
sudo apt update
sudo apt install build-essential libasio-dev wget
```

- Install the SQLite C++ Library
  You need to install the SQLite development files on your Linux system so g++ can link against them.

```sh
sudo apt update
sudo apt install libsqlite3-dev
```

- Setup the repository for development _(from scatch)_

```sh
mkdir headers routers controllers services repository tests
```

- Download the Crow header files _(from scatch)_

```sh
wget https://github.com/CrowCpp/Crow/releases/download/v1.3.0/crow_all.h
```

- For complie the main.cpp (entry points)

```sh
g++ main.cpp -lsqlite3 -lpthread -o app
```

- Run the server

```sh
./app
```

## Some Advence Key points

- How to pass context of the middle to controller without templating in main
```cpp
#include "headers/crow_all.h"
#include "middlewares/logger.h"
#include "controllers/UserController.h"

int main() {
    // Register the global LoggerMiddleware
    crow::App<LoggerMiddleware> app;

    // Notice we capture [&app] and take 'req' as a parameter
    CROW_ROUTE(app, "/data")
    ([&app](const crow::request& req) {

        // 1. Extract the context here, where 'app' exists
        auto& log_ctx = app.get_context<LoggerMiddleware>(req);

        // 2. Pass the extracted data directly to your Controller function!
        return UserController::getSecureData(req, log_ctx.request_id);

    });

    app.port(8080).multithreaded().run();
    return 0;
}
```
----

## Order of resolve the middleware
The execution order has **absolutely nothing to do with the macro**. It is strictly determined by the order you define them in the `AppType` template.

### The Compile-Time Tuple

When you write this in your C++ code:

```cpp
using AppType = crow::App<Logger, RateLimiter, AdminAuth>;

```

The C++ compiler creates a fixed `std::tuple` in memory: `[0: Logger, 1: RateLimiter, 2: AdminAuth]`.

When a request hits your server, Crow runs a simple `for` loop from index `0` to the end of that tuple. If a middleware is Global, it runs it. If a middleware is Local, Crow checks a hidden boolean array to see if you enabled it for this specific route.

Because it iterates through the fixed tuple, **the order in the macro does not matter at all.**

### Example of the "Illusion"

Look at this router code:

```cpp
// You write the macro in reverse order:
bp_admin->CROW_MIDDLEWARES(app, AdminAuth, RateLimiter);

```

Even though you typed `AdminAuth` first in the macro, Crow ignores your typing order. It looks at the master tuple (`Logger -> RateLimiter -> AdminAuth`), loops through it, and says:

1. Is `Logger` enabled? Yes (Global). -> **Runs Logger.**
2. Is `RateLimiter` enabled? Yes (Enabled by macro). -> **Runs RateLimiter.**
3. Is `AdminAuth` enabled? Yes (Enabled by macro). -> **Runs AdminAuth.**

### How to design your `AppType`

Because the `AppType` template is the absolute master controller of the U-Turn sequence, you must design it carefully from "Outer" to "Inner".

The industry standard order for an enterprise `AppType` definition is:

1. **Error Handlers / Crash Recovery** (Outer-most layer)
2. **Loggers** (Record the request immediately)
3. **Security Headers / CORS** (Reject bad browsers early)
4. **Rate Limiters** (Reject spam early to save CPU)
5. **Authentication** (Verify the user's identity)
6. **Authorization** (Verify the user's admin privileges - Inner-most layer)

So your master definition should always look something like this:

```cpp
using AppType = crow::App<Logger, CORS, RateLimiter, UserAuth, AdminAuth>;

```

----


## Query Params

- get the query params by `req.url_params.get("keyword")`

```cpp
inline std::string keyword(const crow::request &req) {
    auto keyword = req.url_params.get("keyword");
    if (keyword != nullptr) {
        return "You are searching for: " + std::string(keyword) + "\n";
    } else {
        return std::string("No search keyword provided.\n");
    }
}
```
