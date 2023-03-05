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
		std::weak_ptr<Render2DComponent> textureComponent = go->AddComponent<Render2DComponent>();
		textureComponent.lock()->SetTexture("background.tga");
		pScene->Add(go);
	}


	// texure 1 LOGO
	{
		go = std::make_shared<dae::GameObject>(pScene);
		std::weak_ptr<Render2DComponent> textureComponent = go->AddComponent<Render2DComponent>();
		textureComponent.lock()->SetTexture("logo.tga");
		textureComponent.lock()->SetPosition(216, 180);
		pScene->Add(go);
	}


	// text comp 1
	{
		auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
		go = std::make_shared<dae::GameObject>(pScene);

		auto textComponent = go->AddComponent<TextComponent>("Programming 4 Assignment", font);
		textComponent.lock()->SetPosition(80, 20);
		pScene->Add(go);
	}


	// text comp 2
	{
		go = std::make_shared<dae::GameObject>(pScene);

		auto font = dae::ResourceManager::GetInstance().LoadFont("lowres.ttf", 36);
		auto textComponent = go->AddComponent<TextComponent>("Press [PAGE UP|DOWN] to change scene", font);
		textComponent.lock()->SetPosition(60, 420);
		textComponent.lock()->SetColor(255, 160, 100, 255);

		pScene->Add(go);
	}

	std::weak_ptr<TransformComponent> transComp{};
	// fps comp 1
	{
		go = std::make_shared<dae::GameObject>(pScene);
		transComp = go->GetComponent<TransformComponent>();
		auto font = dae::ResourceManager::GetInstance().LoadFont("raju-bold.otf", 42);
		std::weak_ptr<FpsComponent> fpsComponent = go->AddComponent<FpsComponent>(font);
		go->SetPosition(270, 80);
		fpsComponent.lock()->SetColor(255, 255, 255, 255);

		pScene->Add(go);
	}

	// orbiting texture 1
	{
		go = std::make_shared<dae::GameObject>(pScene);
		auto textureComponent = go->AddComponent<Render2DComponent>();
		textureComponent.lock()->SetTexture("cheff.png");
		textureComponent.lock()->SetPosition(300, 300);
		textureComponent.lock()->SetResolution(50, 50);
		textureComponent.lock()->SetDrawStyle(Render2DComponent::DrawStyle::positionScale);

		auto orbitComp = go->AddComponent<OrbitComponent>();
		orbitComp.lock()->SetSpeed(-2.6f);
		orbitComp.lock()->SetRadius(50.f);
		pScene->Add(go);
	}

	// orbiting texture 2
	{
		auto parentGo = go; // previous go from RAII block above
		go = std::make_shared<dae::GameObject>(pScene);

		auto textureComponent = go->AddComponent<Render2DComponent>();
		textureComponent.lock()->SetTexture("bean.png");
		textureComponent.lock()->SetPosition(0, 0);
		textureComponent.lock()->SetResolution(50, 50);
		textureComponent.lock()->SetDrawStyle(Render2DComponent::DrawStyle::positionScale);

		go->SetParent(parentGo.get(), true);
		auto orbitComp = go->AddComponent<OrbitComponent>();
		orbitComp.lock()->SetSpeed(6.4f);
		orbitComp.lock()->SetRadius(75.f);
		pScene->Add(go);
	}
}

void dae::SceneFactory::InitFpsDemoScene()
{
	Scene* pScene = dae::SceneManager::GetInstance().AddGameScene("Demo2").get();

	auto font = dae::ResourceManager::GetInstance().LoadFont("raju-bold.otf", 72);
	std::shared_ptr<dae::GameObject> fpsGO = std::make_shared<dae::GameObject>(pScene);


	std::weak_ptr<FpsComponent> fpsComponent = fpsGO->AddComponent<FpsComponent>(font);
	fpsComponent.lock()->SetPosition(200, 180);
	fpsComponent.lock()->SetColor(200, 255, 255, 170);

	std::shared_ptr<GameObject> go = std::make_shared<dae::GameObject>(pScene);
	std::weak_ptr<Render2DComponent> textureComponent = go->AddComponent<Render2DComponent>();
	textureComponent.lock()->SetTexture("colorbars.png");
	textureComponent.lock()->SetDrawStyle(dae::Render2DComponent::DrawStyle::background);
	pScene->Add(go);


	pScene->Add(fpsGO);


}

void dae::SceneFactory::InitBonusScene()
{
	Scene* pScene = dae::SceneManager::GetInstance().AddGameScene("Bonus").get();

	std::shared_ptr<GameObject> go;
	{
		go = std::make_shared<dae::GameObject>(pScene);
		std::weak_ptr<Render2DComponent> textureComponent = go->AddComponent<Render2DComponent>();
		textureComponent.lock()->SetTexture("backdrop_trees.png");
		textureComponent.lock()->SetPosition(-80, 0);
		textureComponent.lock()->SetResolution(800, 600);
		textureComponent.lock()->SetDrawStyle(dae::Render2DComponent::DrawStyle::positionScale);
		Renderer::GetInstance().SetBackgroundColor(SDL_Color{ 100,140,230,255 });
		pScene->Add(go);
	}
	// orbiting texture 1
	{
		go = std::make_shared<dae::GameObject>(pScene);
		auto textureComponent = go->AddComponent<Render2DComponent>();
		textureComponent.lock()->SetTexture("fatalerror.png");
		textureComponent.lock()->SetPosition(300, 200);
		textureComponent.lock()->SetResolution(100, 100);
		textureComponent.lock()->SetDrawStyle(Render2DComponent::DrawStyle::positionScale);

		auto orbitComp = go->AddComponent<OrbitComponent>();
		orbitComp.lock()->SetSpeed(2.6f);
		orbitComp.lock()->SetRadius(100.f);
		pScene->Add(go);
	}

	// orbiting texture 2
	{
		auto parentGo = go; // previous go from RAII block above
		go = std::make_shared<dae::GameObject>(pScene);

		auto textureComponent = go->AddComponent<Render2DComponent>();
		textureComponent.lock()->SetTexture("colorbars.png");
		textureComponent.lock()->SetPosition(0, 0);
		textureComponent.lock()->SetResolution(100, 100);
		textureComponent.lock()->SetDrawStyle(Render2DComponent::DrawStyle::positionScale);

		go->SetParent(parentGo.get(), true);
		auto orbitComp = go->AddComponent<OrbitComponent>();
		orbitComp.lock()->SetSpeed(8.4f);
		orbitComp.lock()->SetRadius(150.f);
		pScene->Add(go);
	}
}
