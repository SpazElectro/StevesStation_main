#include "pch.h"
#include "global_no_headers.h"

#ifndef GLOBAL_H
#define GLOBAL_H

Font fonts[20] = { 0 };

std::vector<ApplicationOrGame> games {};
std::vector<ApplicationOrGame> apps {};

Texture2D backgroundImage;
Audio* selectSound;

const Color highlightColor = BLUE;

#endif