#include <iostream>
#include <vector>
#include <cpr/cpr.h>
#include <json.hpp>

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

// Function to fetch airplane data
std::vector<Airplane> fetchAirplaneData(int latitude, int longitude, int radius) {
    std::vector<Airplane> airplanes;
    
    // Construct API URL
    std::string url = "https://api.airplanes.live/v2/point/" + 
                      std::to_string(latitude) + "/" + 
                      std::to_string(longitude) + "/" + 
                      std::to_string(radius);
    
    // Fetch data
    cpr::Response response = cpr::Get(cpr::Url{url});
    
    // Check response
    if (response.status_code == 200) {
        try {
            // Parse JSON response
            auto json_data = nlohmann::json::parse(response.text);
            
            // Extract airplane data from "ac" field
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

                // Store in the list
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

// Main function to test fetching
int main() {
    int latitude = 40;
    int longitude = -74;
    int radius = 1000;

    std::vector<Airplane> airplanes = fetchAirplaneData(latitude, longitude, radius);

    // Print fetched airplanes
    for (const auto& plane : airplanes) {
        std::cout << "Flight: " << plane.flight << " | "
                  << "Lat: " << plane.latitude << ", Lon: " << plane.longitude << " | "
                  << "Alt: " << plane.altitude << "ft | Speed: " << plane.speed << "kt | "
                  << "Type: " << plane.type << " | Airline: " << plane.airline << std::endl;
    }

    return 0;
}
