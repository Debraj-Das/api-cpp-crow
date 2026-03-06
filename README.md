# Back-end Development using C++ CROW
Setting up a CRUD API with Crow on a Linux system is remarkable straight forward. Because Crow is a "header-only" framework, you do not have to go through complex installation processes or build massive libraries; you mostly just need the header file and a standard system dependency.

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

- Setup the repository for development *(from scatch)*
```sh
mkdir headers controllers services repository tests
```

- Download the Crow header files *(from scatch)*
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
