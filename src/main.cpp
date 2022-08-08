#include "global.h"
#include "global_no_headers.h"

// TO DO: mouse hover selection
// and game scrolling

void drawAppsOrGames() {
    int paddingX = GAMES_STARTING_POINT;
    int paddingY = GAMES_STARTING_Y_POINT;

    std::vector<ApplicationOrGame> gamesOrApps;

    if(itemSelected == 0) {
        gamesOrApps = games;
    } else if(itemSelected == 1) {
        gamesOrApps = apps;
    }

    for (const auto& asset : gamesOrApps) {
        DrawTexture(asset.texture, paddingX, paddingY, WHITE);

        if(optionSelected == asset.id) {
            DrawRectangleLinesEx(quickRect(
                quickVec(paddingX, paddingY),
                quickVec(GAME_WIDTH, GAME_HEIGHT)
            ), 5, highlightColor);

            if(showGameMenu) {
                DrawRectangle(WINDOW_WIDTH / 2 - 100, 650, BUTTON_WIDTH, BUTTON_HEIGHT, GRAY); 
                DrawRectangle(WINDOW_WIDTH / 2 + 100, 650, BUTTON_WIDTH, BUTTON_HEIGHT, GRAY); 

                int xPos = 0;

                if(gameMenuItemSelected == 0) { xPos = WINDOW_WIDTH / 2 - 100; }
                if(gameMenuItemSelected == 1) { xPos = WINDOW_WIDTH / 2 + 100; }

                DrawRectangleLinesEx(quickRect(
                    quickVec(xPos, 650),
                    quickVec(BUTTON_WIDTH, BUTTON_HEIGHT)
                ), 5, highlightColor);

                if(gameMenuItemSelected == 0) {
                    DrawRectangleLinesEx(quickRect(
                        quickVec(WINDOW_WIDTH / 2 + 100, 650),
                        quickVec(BUTTON_WIDTH, BUTTON_HEIGHT)
                    ), 5, BLACK);
                } else if(gameMenuItemSelected == 1) {
                    DrawRectangleLinesEx(quickRect(
                        quickVec(WINDOW_WIDTH / 2 - 100, 650),
                        quickVec(BUTTON_WIDTH, BUTTON_HEIGHT)
                    ), 5, BLACK);
                }

                DrawTextEx(fonts[1], "Play", quickVec(WINDOW_WIDTH / 2 - 100+(BUTTON_WIDTH/2), 650+(BUTTON_HEIGHT/2)), 15, 1, BLACK);
                DrawTextEx(fonts[1], "Exit", quickVec(WINDOW_WIDTH / 2 + 100+(BUTTON_WIDTH/2), 650+(BUTTON_HEIGHT/2)), 15, 1, BLACK);
            } else {
                DrawTextEx(fonts[1], asset.description.c_str(), quickVec(0, 610), 20, 1, WHITE);
            }
        }

        paddingX += GAME_WIDTH;

        if(paddingX + GAME_WIDTH >= WINDOW_WIDTH) {
            paddingX = GAMES_STARTING_POINT;
            paddingY += GAME_HEIGHT;
        }
    }
}

void setupGamesAndApplications() {
    std::fstream appsData;
    appsData.open("apps.ssd");

    std::string data;

    if(appsData.is_open()) {
        ApplicationOrGame* currentGameOrApp = new ApplicationOrGame();
        int uid = 0;

        std::string type;
        std::string line;
            
        while(std::getline(appsData, line)) {
            data += line + "\n";

            std::istringstream iss(line);
            std::string token;

            while (std::getline(iss, token, '"'))
            {
                if(token != "" and token != "\n" and token != " ") {
                    uid += 1;

                    if(uid == 1) {
                        if(token == "app" or token == "game") {
                            type = token;
                        } else {
                            std::cout << "Invalid type" << std::endl;
                        }
                    } else if(uid == 2) {
                        currentGameOrApp->name = token;
                    } else if(uid == 3) {
                        currentGameOrApp->gamePath = token;
                    }
                }

                if(uid == 3) {
                    uid = 0;
                    break;
                }
            }

            if(uid != 0) {
                std::cout << "Error while parsing file, not enough arguments" << std::endl;
            } else {
                ApplicationOrGame* gameApp = new ApplicationOrGame(currentGameOrApp->name, games.size());
                gameApp->gamePath = currentGameOrApp->gamePath;

                if(type == "game") {
                    games.push_back(*(gameApp));
                } else if(type == "app") {
                    gameApp->id = apps.size();
                    apps.push_back(*(gameApp));
                } else {
                    std::cout << "Error: Application type is invalid" << std::endl;
                }
            }
        }

        appsData.close();
    } else {
        std::cout << "Error while trying to open games data file" << std::endl;
    }
}

void launchCurrentGame() {
    std::string path = "";

    if(itemSelected == 0) {
        if(optionSelected < games.size()) {
            path = games.at(optionSelected).gamePath;
        }
    } else if(itemSelected == 1) {
        if(optionSelected < apps.size()) {
            path = apps.at(optionSelected).gamePath;
        }
    }

    if(!(path == "")) {
        selectSound->PlayMulti();

        // the "if 1==1" might seem dumb but
        // cmd actually removes the quotes for some dumb reason
        // so we add this to sort of bypass it
        system((("if 1==1 \"" + gameLauncherPath + "\" \"") + path + "\"").c_str());
    }
}

int main() {
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
    InitAudioDevice();
    
    // essentials
    backgroundImage = getResizedTextureFromPath("resources/background.png", WINDOW_WIDTH, WINDOW_HEIGHT);
    selectSound = new Audio("sounds/select.wav");

    fonts[0] = LoadFont("fonts/nokiafc22.ttf");
    fonts[1] = LoadFont("fonts/monsterrat.ttf");
    fonts[2] = LoadFont("fonts/arial.ttf");

    setupGamesAndApplications();

    SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));

    while(!WindowShouldClose()) {
        if(IsWindowFocused()) {
            if(
                IsKeyPressed(KEY_LEFT) or
                IsGamepadButtonPressed(0, 4) or IsGamepadButtonPressed(1, 4)
            ) {
                if(showGameMenu) {
                    gameMenuItemSelected -= 1;
                } else {
                    optionSelected -= 1;
                }
            }

            if(
                IsKeyPressed(KEY_RIGHT) or
                IsGamepadButtonPressed(0, 2) or IsGamepadButtonPressed(1, 2)
            ) {
                if(showGameMenu) {
                    gameMenuItemSelected += 1;
                } else {
                    optionSelected += 1;
                }
            }
            
            if(
                (IsKeyPressed(KEY_UP) or
                IsGamepadButtonPressed(0, 1) or IsGamepadButtonPressed(1, 1)) and
                not showGameMenu
            ) {
                optionSelected = 0;
                itemSelected -= 1;
            }

            if(
                (IsKeyPressed(KEY_DOWN) or
                IsGamepadButtonPressed(0, 3) or IsGamepadButtonPressed(1, 3)) and
                not showGameMenu
            ) {
                optionSelected = 0;
                itemSelected += 1;
            }

            if(
                (IsKeyPressed(KEY_ENTER) or IsKeyPressed(KEY_SPACE) or IsKeyPressed(KEY_KP_ENTER) or
                IsGamepadButtonPressed(0, 7) or IsGamepadButtonPressed(1, 7)) and
                (itemSelected == 0 or itemSelected == 1)
            ) {
                bool allowed = false;

                if(itemSelected == 0) {
                    if(optionSelected < games.size()) {
                        allowed = true;
                    }
                } else if(itemSelected == 1) {
                    if(optionSelected < apps.size()) {
                        allowed = true;
                    }
                }

                if(not showGameMenu and allowed) {
                    gameMenuItemSelected = 0;
                    showGameMenu = true;
                } else if(showGameMenu and allowed) {
                    // 0 play
                    // 1 exit

                    if(gameMenuItemSelected == 0) {
                        launchCurrentGame();
                        showGameMenu = false;
                    } else if(gameMenuItemSelected == 1) {
                        showGameMenu = false;
                    }
                }
            }
            
            if(itemSelected < 0) itemSelected = 3;
            if(itemSelected > 3) itemSelected = 0;

            if(gameMenuItemSelected >= 2) gameMenuItemSelected = 0;
            if(gameMenuItemSelected < 0) gameMenuItemSelected = 1;

            if(itemSelected == 0) {
                if(optionSelected < 0) {
                    optionSelected = games.size() - 1;
                }

                if(optionSelected >= games.size()) {
                    optionSelected = 0;
                }
            } else if(itemSelected == 1) {
                if(optionSelected < 0) {
                    optionSelected = apps.size() - 1;
                }

                if(optionSelected >= apps.size()) {
                    optionSelected = 0;
                }
            }
        }
        
        BeginDrawing();
            ClearBackground(GRAY);

            DrawTexture(backgroundImage, 0, 0, WHITE);
            DrawFPS(WINDOW_WIDTH - 100, WINDOW_HEIGHT - 20);

            // Buttons

            if(itemSelected == 0) DrawRectangle(35, 100, 115, 50, highlightColor);
            DrawTextEx(fonts[0], "GAMES", quickVec(35+15, 115), 20, 1, WHITE);

            if(itemSelected == 1) DrawRectangle(35, 150, 115, 50, highlightColor);
            DrawTextEx(fonts[0], "APPS", quickVec(35+15, 165), 20, 1, WHITE);

            if(itemSelected == 2) DrawRectangle(35, 200, 115, 50, highlightColor);
            DrawTextEx(fonts[0], "STORE", quickVec(35+15, 215), 20, 1, WHITE);

            if(itemSelected == 3) DrawRectangle(35, 250, 115, 50, highlightColor);
            DrawTextEx(fonts[0], "SETTINGS", quickVec(35+15, 265), 20, 1, WHITE);

            // Text

            DrawTextEx(
                fonts[1], (std::to_string(GPAmount) + " GP").c_str(),
                quickVec(
                    (WINDOW_WIDTH - (username.length() * 25) - (std::to_string(GPAmount).length() * 25)) - 125,
                    10), 25, 1, RAYWHITE
            );
            
            DrawTextEx(
                fonts[1], username.c_str(),
                quickVec(
                WINDOW_WIDTH - (username.length() * 30),
                10), 25, 1, RAYWHITE);

            // Content

            if(itemSelected == 0 or itemSelected == 1) drawAppsOrGames();

        EndDrawing();
    }

    UnloadTexture(backgroundImage);

    for (const auto& game : games) {
        UnloadTexture(game.texture);
    }

    for (const auto& app : apps) {
        UnloadTexture(app.texture);
    }

    UnloadSound(selectSound->sound);
    
    CloseAudioDevice();
    CloseWindow();

    return 0;
}
