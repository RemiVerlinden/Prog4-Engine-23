#include "SceneFactory.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "TextComponent.h"
#include "FpsComponent.h"
#include "Render2DComponent.h"
#include "GameObject.h"

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

	std::shared_ptr<GameObject> go = std::make_shared<dae::GameObject>();
	std::weak_ptr<Render2DComponent> textureComponent = go->AddComponent<Render2DComponent>();
	textureComponent.lock()->SetTexture("background.tga");
	pScene->Add(go);


	go = std::make_shared<dae::GameObject>();
	textureComponent = go->AddComponent<Render2DComponent>();
	textureComponent.lock()->SetTexture("logo.tga");
	textureComponent.lock()->SetPosition(216, 180);
	pScene->Add(go);


	// text comp 1
	auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	go = std::make_shared<dae::GameObject>();
	
	auto textComponent = go->AddComponent<TextComponent>("Programming 4 Assignment", font);
	textComponent.lock()->SetPosition(80, 20);
	pScene->Add(go);

	go = std::make_shared<dae::GameObject>();

	// text comp 2
	go = std::make_shared<dae::GameObject>();

	font = dae::ResourceManager::GetInstance().LoadFont("lowres.ttf", 36);
	textComponent = go->AddComponent<TextComponent>("Press [PAGE UP|DOWN] to change scene", font);
	textComponent.lock()->SetPosition(60, 420);
	textComponent.lock()->SetColor(255,160,100,255);
	
	pScene->Add(go);

	// fps comp
	go = std::make_shared<dae::GameObject>();

	font = dae::ResourceManager::GetInstance().LoadFont("raju-bold.otf", 42);
	std::weak_ptr<FpsComponent> fpsComponent = go->AddComponent<FpsComponent>(font);
	fpsComponent.lock()->SetPosition(250, 300);
	fpsComponent.lock()->SetColor(255, 255, 255, 255);
	pScene->Add(go);
}

void dae::SceneFactory::InitTextDemoScene()
{
	auto pScene = dae::SceneManager::GetInstance().AddGameScene("Demo2");

	auto font = dae::ResourceManager::GetInstance().LoadFont("raju-bold.otf", 72);
	std::shared_ptr<dae::GameObject> fpsGO = std::make_shared<dae::GameObject>();


	std::weak_ptr<FpsComponent> fpsComponent = fpsGO->AddComponent<FpsComponent>(font);
	fpsComponent.lock()->SetPosition(200, 180);
	fpsComponent.lock()->SetColor(200,255,200,255);


	pScene->Add(fpsGO);
}
