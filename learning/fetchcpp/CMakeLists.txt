# 1. State the minimum CMake version required (usually 3.10 or higher)
cmake_minimum_required(VERSION 3.15)

# 2. Name your project and optionally set the C++ standard (like C++17)
project(MyAwesomeServer)
set(CMAKE_CXX_STANDARD 17)

# 3. Tell CMake what libraries you need from your computer
# (REQUIRED means CMake will stop and warn you if it can't find them)
find_package(cpr REQUIRED)
find_package(Crow REQUIRED)

# 4. Create your executable (Name it 'server', built from 'main.cpp')
add_executable(server main.cpp)

# 5. Link the libraries to your executable
target_link_libraries(server PRIVATE cpr::cpr Crow::Crow)
