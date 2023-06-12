#include <filesystem>
#include "Common.h"
#include "Locator.h"
#include <SDL.h>

using namespace engine::physics;

bool fileExists(const std::string& path)
{
    if (std::filesystem::exists(path))
        return true;
    else 
    {
        ENGINE_ERROR("path to file {} does not exist!", path);
        return false;
    }
}

std::string UniqueName()
{
	static int counter = 0;
	return std::to_string(counter++);
}

SDL_Rect ToSDLRect(const Box& box)
{
	return SDL_Rect{ static_cast<int>(box.pos.x), static_cast<int>(box.pos.y), static_cast<int>(box.size.x), static_cast<int>(box.size.y) };
}