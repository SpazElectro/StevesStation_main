#include <iostream>
#include <vector>
#include <fstream>
#include <cstring>  
#include <sstream>
#include <filesystem>

#include "global_no_headers.h"

extern "C"
{
#include "raylib.h"
}

#ifndef PCH_H
#define PCH_H

Vector2 quickVec(int x, int y) {
    Vector2 pos = Vector2();
    pos.x = x;
    pos.y = y;

    return pos;
}

Rectangle quickRect(Vector2 position, Vector2 scale) {
    Rectangle rect;
    rect.x = position.x; rect.y = position.y;
    rect.width = scale.x; rect.height = scale.y;

    return rect;
}

Texture2D getTextureFromPath(const char* path) {
    Image image = LoadImage(path);
    Texture2D texture = LoadTextureFromImage(image);
    UnloadImage(image);

    return texture;
}

Texture2D getResizedTextureFromPath(const char* path, int newWidth, int newHeight) {
    Image image = LoadImage(path);

    ImageResize((&image), newWidth, newHeight);

    Texture2D texture = LoadTextureFromImage(image);
    UnloadImage(image);

    return texture;
}

class ApplicationOrGame {
public:
    std::string name;
    std::string description;
    std::string gamePath;
    int id;

    Image image;
    Texture2D texture;

    ApplicationOrGame() {
        name = "Unnamed";
        description = "Description is not defined";
        id = 0;
        gamePath = "";
    }

    ApplicationOrGame(std::string name, int id) {
        this->name = name;

        std::fstream file;
        file.open("applicationData/" + name + "/description.txt");

        std::string description;

        if(file.is_open()) {
            std::string line;
            
            while(getline(file, line)) {
                description += line + "\n";
            }

            file.close();
        } else {
            description = "Description not found";
        }
        
        this->description = description;
        this->image = LoadImage(("applicationData/" + name + "/icon.png").c_str());
        
        ImageResize(
            &(this->image),
            GAME_WIDTH, GAME_HEIGHT
        );

        this->texture = LoadTextureFromImage(this->image);
        UnloadImage(this->image);

        this->id = id;
    }
};

class Audio {
public:
    Sound sound;

    Audio(const char* path) {
        this->sound = LoadSound(path);
    }

    void Play() {
        PlaySound(this->sound);
    }

    void Stop() {
        StopSound(this->sound);
    }

    void PlayMulti() {
        PlaySoundMulti(this->sound);
    }

    void StopMulti() {
        StopSoundMulti();
    }
};

#endif