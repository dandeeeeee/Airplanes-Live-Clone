#include "raylib.h"

#if defined(_WIN32)           
	#define NOGDI            
	#define NOUSER           
#endif

#include <Windows.h> 

#if defined(_WIN32)           
	#undef far
#endif

#include "cpr/cpr.h"
#include "json.hpp"
#include <iostream>

using json = nlohmann::json;

int main() {

    const int screenWidth = 800;
    const int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "AirplanesLive Clone Test");


    cpr::Response r = cpr::Get(cpr::Url{"https://api.github.com/repos/raysan5/raylib"});
    

    if (r.status_code == 200) {
        json j = json::parse(r.text);
        std::string repoName = j["full_name"];
        std::string repoDescription = j["description"];

        std::cout << "Repository: " << repoName << std::endl;
        std::cout << "Description: " << repoDescription << std::endl;
    } else {
        std::cerr << "Failed to fetch data from GitHub API. Status code: " << r.status_code << std::endl;
    }


    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(DARKGREEN);
        EndDrawing();
    }

  
    CloseWindow();

    return 0;
}