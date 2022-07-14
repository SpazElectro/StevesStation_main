#include <iostream>

#ifndef GLOBAL_NO_HEADERS_H
#define GLOBAL_NO_HEADERS_H

int itemSelected = 0;
int optionSelected = 0;

bool showGameMenu = false;
int gameMenuItemSelected = 0;

const std::string username = "Steve";
const int GPAmount = 50000;

const int WINDOW_WIDTH = 1024;
const int WINDOW_HEIGHT = 768;
const char* WINDOW_TITLE = "Steve's Station";

const int GAME_WIDTH = 75;
const int GAME_HEIGHT = 75;
const int GAMES_STARTING_POINT = 172;
const int GAMES_STARTING_Y_POINT = 62;

const int BUTTON_WIDTH = 75;
const int BUTTON_HEIGHT = 75;

const std::string gameLauncherPath = std::filesystem::current_path().string() + "\\gameLauncher.exe";

#endif