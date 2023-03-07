#include "SceneFactory.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Scene.h"
#include "TextComponent.h"
#include "FpsComponent.h"
#include "Render2DComponent.h"
#include "GameObject.h"
#include "OrbitComponent.h"

using namespace dae;

void dae::SceneFactory::CreateScenes()
{
	InitDefaultScene();
	InitFpsDemoScene();
	InitBonusScene();
}

void dae::SceneFactory::InitDefaultScene()
{
	Scene* pScene = dae::SceneManager::GetInstance().AddGameScene("Demo").get();
	dae::SceneManager::GetInstance().SetActiveGameScene("Demo");

	std::shared_ptr<GameObject> go = std::make_shared<dae::GameObject>(pScene);

	// Background
	{
		Render2DComponent* textureComponent = go->AddComponent<Render2DComponent>();
		textureComponent->SetTexture("background.tga");
		pScene->Add(go);
	}


	// texure 1 LOGO
	{
		go = std::make_shared<dae::GameObject>(pScene);
		Render2DComponent* textureComponent = go->AddComponent<Render2DComponent>();
		textureComponent->SetTexture("logo.tga");
		textureComponent->SetPosition(216, 180);
		pScene->Add(go);
	}


	// text comp 1
	{
		auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
		go = std::make_shared<dae::GameObject>(pScene);

		auto textComponent = go->AddComponent<TextComponent>("Programming 4 Assignment", font);
		textComponent->SetPosition(80, 20);
		pScene->Add(go);
	}


	// text comp 2
	{
		go = std::make_shared<dae::GameObject>(pScene);

		auto font = dae::ResourceManager::GetInstance().LoadFont("lowres.ttf", 36);
		auto textComponent = go->AddComponent<TextComponent>("Press [PAGE UP|DOWN] to change scene", font);
		textComponent->SetPosition(60, 420);
		textComponent->SetColor(255, 160, 100, 255);

		pScene->Add(go);
	}

	// fps comp 1
	{
		go = std::make_shared<dae::GameObject>(pScene);
		auto font = dae::ResourceManager::GetInstance().LoadFont("raju-bold.otf", 42);
		//auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
		FpsComponent* fpsComponent = go->AddComponent<FpsComponent>(font);
		go->SetPosition(270, 80);
		fpsComponent->SetColor(255, 255, 255, 255);

		pScene->Add(go);
	}

	// orbiting texture 1
	{
		go = std::make_shared<dae::GameObject>(pScene);
		auto textureComponent = go->AddComponent<Render2DComponent>();
		textureComponent->SetTexture("cheff.png");
		textureComponent->SetPosition(300, 300);
		textureComponent->SetResolution(50, 50);
		textureComponent->SetDrawStyle(Render2DComponent::DrawStyle::positionScale);

		auto orbitComp = go->AddComponent<OrbitComponent>();
		orbitComp->SetSpeed(-2.6f);
		orbitComp->SetRadius(50.f);
		pScene->Add(go);
	}

	// orbiting texture 2
	{
		auto parentGo = go; // previous go from RAII block above
		go = std::make_shared<dae::GameObject>(pScene);

		auto textureComponent = go->AddComponent<Render2DComponent>();
		textureComponent->SetTexture("bean.png");
		textureComponent->SetPosition(0, 0);
		textureComponent->SetResolution(50, 50);
		textureComponent->SetDrawStyle(Render2DComponent::DrawStyle::positionScale);

		go->SetParent(parentGo.get(), true);
		auto orbitComp = go->AddComponent<OrbitComponent>();
		orbitComp->SetSpeed(6.4f);
		orbitComp->SetRadius(75.f);
		pScene->Add(go);
	}
}

void dae::SceneFactory::InitFpsDemoScene()
{
	Scene* pScene = dae::SceneManager::GetInstance().AddGameScene("Demo2").get();

	auto font = dae::ResourceManager::GetInstance().LoadFont("raju-bold.otf", 72);
	std::shared_ptr<dae::GameObject> fpsGO = std::make_shared<dae::GameObject>(pScene);


	FpsComponent* fpsComponent = fpsGO->AddComponent<FpsComponent>(font);
	fpsComponent->SetPosition(200, 180);
	fpsComponent->SetColor(200, 255, 255, 170);

	std::shared_ptr<GameObject> go = std::make_shared<dae::GameObject>(pScene);
	Render2DComponent* textureComponent = go->AddComponent<Render2DComponent>();
	textureComponent->SetTexture("colorbars.png");
	textureComponent->SetDrawStyle(dae::Render2DComponent::DrawStyle::background);
	pScene->Add(go);


	pScene->Add(fpsGO);


}

void dae::SceneFactory::InitBonusScene()
{
	Scene* pScene = dae::SceneManager::GetInstance().AddGameScene("Bonus").get();

	std::shared_ptr<GameObject> go;
	{
		go = std::make_shared<dae::GameObject>(pScene);
		Render2DComponent* textureComponent = go->AddComponent<Render2DComponent>();
		textureComponent->SetTexture("backdrop_trees.png");
		textureComponent->SetPosition(-80, 0);
		textureComponent->SetResolution(800, 600);
		textureComponent->SetDrawStyle(dae::Render2DComponent::DrawStyle::positionScale);
		Renderer::GetInstance().SetBackgroundColor(SDL_Color{ 100,140,230,255 });
		pScene->Add(go);
	}
	// orbiting texture 1
	{
		go = std::make_shared<dae::GameObject>(pScene);
		auto textureComponent = go->AddComponent<Render2DComponent>();
		textureComponent->SetTexture("fatalerror.png");
		textureComponent->SetPosition(300, 200);
		textureComponent->SetResolution(100, 100);
		textureComponent->SetDrawStyle(Render2DComponent::DrawStyle::positionScale);

		auto orbitComp = go->AddComponent<OrbitComponent>();
		orbitComp->SetSpeed(2.6f);
		orbitComp->SetRadius(100.f);
		pScene->Add(go);
	}

	// orbiting texture 2
	{
		auto parentGo = go; // previous go from RAII block above
		go = std::make_shared<dae::GameObject>(pScene);

		auto textureComponent = go->AddComponent<Render2DComponent>();
		textureComponent->SetTexture("colorbars.png");
		textureComponent->SetPosition(0, 0);
		textureComponent->SetResolution(100, 100);
		textureComponent->SetDrawStyle(Render2DComponent::DrawStyle::positionScale);

		go->SetParent(parentGo.get(), true);
		auto orbitComp = go->AddComponent<OrbitComponent>();
		orbitComp->SetSpeed(8.4f);
		orbitComp->SetRadius(150.f);
		pScene->Add(go);
	}
}
