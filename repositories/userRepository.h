#pragma once
#include <bits/stdc++.h>
#include <mutex> // Required for thread safety
#include <sqlite3.h>

namespace UserRepository {

struct User {
    int id;
    std::string username;
    std::string age;
};

// 1. Global static pointers for the persistent connection and the lock
const static char *db_filename = "app.db";
static sqlite3 *db = nullptr;
static std::mutex db_mutex;

// 2. Open the database ONCE
inline bool init_database() {
    if (sqlite3_open(db_filename, &db) != SQLITE_OK) {
        std::cerr << "Failed to open database" << std::endl;
        return false;
    }

    const char *sql = "CREATE TABLE IF NOT EXISTS USERS("
                      "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
                      "USERNAME TEXT NOT NULL, "
                      "AGE TEXT NOT NULL);";

    // No lock needed here as this runs before the server starts accepting
    // requests
    sqlite3_exec(db, sql, nullptr, nullptr, nullptr);
    return true;
}

// 3. Optional: Close it gracefully when the server shuts down
inline void close_database() {
    if (db) {
        sqlite3_close(db);
        db = nullptr;
    }
}

inline bool create_user(const User &user) {
    // Lock the mutex. It automatically unlocks when the function finishes
    // (RAII).
    std::lock_guard<std::mutex> lock(db_mutex);

    std::string sql = "INSERT INTO USERS (USERNAME, AGE) VALUES ('" +
                      user.username + "', '" + user.age + "');";

    int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, nullptr);
    return rc == SQLITE_OK;
}

inline int fetch_users_callback(void *data, int argc, char **argv,
                                char **azColName) {
    auto *users = static_cast<std::vector<User> *>(data);
    User u;
    for (int i = 0; i < argc; i++) {
        std::string colName = azColName[i];
        if (colName == "ID")
            u.id = argv[i] ? std::stoi(argv[i]) : 0;
        else if (colName == "USERNAME")
            u.username = argv[i] ? argv[i] : "";
        else if (colName == "AGE")
            u.age = argv[i] ? argv[i] : "";
    }
    users->push_back(u);
    return 0;
}

inline std::vector<User> get_all_users() {
    std::lock_guard<std::mutex> lock(db_mutex); // Lock for safe reading

    std::vector<User> users;
    const char *sql = "SELECT * FROM USERS;";
    sqlite3_exec(db, sql, fetch_users_callback, &users, nullptr);

    return users;
}

inline bool update_user(int id, const User &user) {
    std::lock_guard<std::mutex> lock(db_mutex); // Lock for safe writing

    std::string sql = "UPDATE USERS SET USERNAME = '" + user.username +
                      "', AGE = '" + user.age +
                      "' WHERE ID = " + std::to_string(id) + ";";

    int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, nullptr);
    return rc == SQLITE_OK;
}

inline bool delete_user(int id) {
    std::lock_guard<std::mutex> lock(db_mutex); // Lock for safe writing

    std::string sql =
        "DELETE FROM USERS WHERE ID = " + std::to_string(id) + ";";
    int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, nullptr);
    return rc == SQLITE_OK;
}

inline std::optional<User> get_user_by_id(int id) {
    std::lock_guard<std::mutex> lock(db_mutex); // Thread-safe read lock

    std::vector<User> users; // We will temporarily store the result here

    // std::to_string guarantees this is a pure number, making it safe
    std::string sql =
        "SELECT * FROM USERS WHERE ID = " + std::to_string(id) + ";";

    // Execute the query. If found, the callback will push 1 user into the
    // vector.
    sqlite3_exec(db, sql.c_str(), fetch_users_callback, &users, nullptr);

    // Check if we actually found someone
    if (!users.empty()) {
        return users[0]; // Return the first (and only) user found
    }

    return std::nullopt; // Return "Nothing" if the ID doesn't exist
}

} // namespace UserRepository
