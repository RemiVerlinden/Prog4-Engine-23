#pragma once
#include <string>
#include "Structs.h"

struct SDL_Rect;

bool fileExists(const std::string& path);

std::string UniqueName();

SDL_Rect ToSDLRect(const engine::physics::Box& box);
