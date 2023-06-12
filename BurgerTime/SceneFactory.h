#pragma once
#include <string>

namespace engine
{
	class Scene;
	namespace SceneFactory
	{
		void CreateScenes();

		void InitSceneKeybinds();

		Scene* InitBurgerTimeStage(const std::string& stage, bool setActive);

		Scene* InitMainMenu();
		Scene* InitEndScreen();

	}
}