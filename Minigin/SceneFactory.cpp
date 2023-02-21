#include "SceneFactory.h"
#include "SceneManager.h"
#include "TextObject.h"
#include "ResourceManager.h"
#include "Scene.h"

using namespace dae;

void dae::SceneFactory::CreateScenes()
{
	InitDefaultScene();
	InitTextDemoScene();
}

void dae::SceneFactory::InitDefaultScene()
{
	auto& pScene = dae::SceneManager::GetInstance().AddGameScene("Demo");
	dae::SceneManager::GetInstance().SetActiveGameScene("Demo");

	auto go = std::make_shared<dae::GameObject>();
	go->SetTexture("background.tga");
	pScene->Add(go);
	

	go = std::make_shared<dae::GameObject>();
	go->SetTexture("logo.tga");
	go->SetPosition(216, 180);
	pScene->Add(go);

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	auto to = std::make_shared<dae::TextObject>("Programming 4 Assignment", font);
	to->SetPosition(80, 20);
	pScene->Add(to);
}

void dae::SceneFactory::InitTextDemoScene()
{
	auto scene = dae::SceneManager::GetInstance().AddGameScene("Demo2");

	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	std::shared_ptr<dae::TextObject> fpsCounter = std::make_shared<dae::TextObject>("0", font);
	fpsCounter->SetPosition(100, 400);

	scene->Add(fpsCounter);
}
