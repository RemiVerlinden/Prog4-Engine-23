#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#define NOMINMAX

#include "Engine2D.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "SceneFactory.h"

void load()
{
	engine::SceneFactory::CreateScenes();
}

int main(int, char*[]) {
	engine::Engine2D engine("../Data/");
	engine.Run(load);
    return 0;
}