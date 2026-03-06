# Back-end Development using C++ CROW

Setting up a CRUD API with Crow on a Linux system is remarkable straight forward. Because Crow is a "header-only" framework, you do not have to go through complex installation processes or build massive libraries; you mostly just need the header file and a standard system dependency.

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
wget https://github.com/CrowCpp/Crow/releases/download/v1.2.0/crow_all.h
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
