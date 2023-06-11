#include <filesystem>
#include "Common.h"
#include "Locator.h"

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