#include "raylib.h"

#if defined(_WIN32)           
	#define NOGDI            
	#define NOUSER           
#endif

#include <Windows.h> 

#if defined(_WIN32)           
	#undef far
#endif

#include <iostream>
#include <vector>
#include <cpr/cpr.h>
#include <json.hpp>
#include <chrono>

// Airplane struct to hold flight data
struct Airplane {
    std::string flight;
    double latitude;
    double longitude;
    double altitude;
    double speed;
    double direction;
    std::string type;
    std::string airline;
    std::string hex;
};

// Converts latitude/longitude to screen coordinates
Vector2 ConvertToScreen(double lat, double lon, double mapMinLat, double mapMaxLat, double mapMinLon, double mapMaxLon, int screenWidth, int screenHeight) {
    float x = ((lon - mapMinLon) / (mapMaxLon - mapMinLon)) * screenWidth;
    float y = ((lat - mapMinLat) / (mapMaxLat - mapMinLat)) * screenHeight;
    return { x, screenHeight - y }; // Flip Y-axis to match screen coordinates
}

// Function to fetch airplane data
std::vector<Airplane> fetchAirplaneData(int latitude, int longitude, int radius) {
    std::vector<Airplane> airplanes;
    std::string url = "https://api.airplanes.live/v2/point/" + std::to_string(latitude) + "/" +
                      std::to_string(longitude) + "/" + std::to_string(radius);

    cpr::Response response = cpr::Get(cpr::Url{url});

    if (response.status_code == 200) {
        try {
            auto json_data = nlohmann::json::parse(response.text);
            for (const auto& item : json_data["ac"]) {
                Airplane plane;
                plane.flight = item.value("flight", "UNKNOWN");
                plane.latitude = item.value("lat", 0.0);
                plane.longitude = item.value("lon", 0.0);
                plane.altitude = item.value("alt_baro", 0.0);
                plane.speed = item.value("gs", 0.0);
                plane.direction = item.value("dir", 0.0);
                plane.type = item.value("desc", "UNKNOWN");
                plane.airline = item.value("ownOp", "UNKNOWN");
                plane.hex = item.value("hex", "UNKNOWN");
                airplanes.push_back(plane);
            }
        } catch (const std::exception &e) {
            std::cerr << "JSON Parsing Error: " << e.what() << std::endl;
        }
    } else {
        std::cerr << "Failed to fetch data. HTTP Status: " << response.status_code << std::endl;
    }
    return airplanes;
}

int main() {
    // Initialize Raylib
    const int screenWidth = 1280;
    const int screenHeight = 720;
    InitWindow(screenWidth, screenHeight, "AirplanesLive Clone");
    SetTargetFPS(60);

    // Initial data fetch
    int latitude = 40;
    int longitude = -74;
    int radius = 1000;
    std::vector<Airplane> airplanes = fetchAirplaneData(latitude, longitude, radius);

    auto lastUpdate = std::chrono::steady_clock::now();
    const int updateInterval = 5; // Fetch new data every 5 seconds

    while (!WindowShouldClose()) {
        auto now = std::chrono::steady_clock::now();
        double elapsed = std::chrono::duration<double>(now - lastUpdate).count();

        if (elapsed > updateInterval) {
            airplanes = fetchAirplaneData(latitude, longitude, radius);
            lastUpdate = now;
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("AirplanesLive Clone - Raylib", 10, 10, 20, DARKGRAY);

        // Render airplanes
        for (const auto& plane : airplanes) {
            Vector2 position = ConvertToScreen(plane.latitude, plane.longitude, 20, 50, -130, -60, screenWidth, screenHeight);
            DrawCircleV(position, 5, RED);
            DrawText(plane.flight.c_str(), position.x + 10, position.y, 10, BLACK);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
