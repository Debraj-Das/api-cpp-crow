#include <crow.h>
#include <cpr/cpr.h>
#include <string>
#include <iostream>

std::string solve(std::string url) {
    // 1. Fetch the data
    cpr::Response r = cpr::Get(cpr::Url{url});
    
    // 2. Check for 200 OK, not 202 ACCEPTED
    if (r.status_code != crow::status::OK) {
        return "Error fetching from API. Status: " + std::to_string(r.status_code);
    }

    // 3. Parse the JSON
    crow::json::rvalue parsed_data = crow::json::load(r.text);
    if (!parsed_data) {
        return "Error: Invalid JSON received.";
    }

    // 4. Safely check which type of joke it is so we don't crash
    std::string joke_type = parsed_data["type"].s();
    std::string result;

    if (joke_type == "twopart") {
        // Two-part jokes have a setup and delivery
        std::string setup = parsed_data["setup"].s();
        std::string delivery = parsed_data["delivery"].s();
        result = setup + " ... " + delivery;
    } else if (joke_type == "single") {
        // Single jokes just have a "joke" field
        result = parsed_data["joke"].s();
    } else {
        result = "Unknown joke type.";
    }

    return result;
}

constexpr int PORT = 8080;

int main() {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")([](){
        std::string s = solve("https://v2.jokeapi.dev/joke/Programming");
        // It is also best practice to return OK (200) to the browser instead of ACCEPTED (202)
        return crow::response(crow::status::OK, s);
    });

    app.port(PORT).multithreaded().run();

    return 0;
}
