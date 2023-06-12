#include "GameOverComponent.h"
#include "SceneManager.h"
#include "Locator.h"

using namespace engine;

void GameOverComponent::Initialize()
{
	EventSubscribe(OnGameOver);
}

void GameOverComponent::OnEvent(const engine::Event& sent)
{
	if (sent.GetEventType() == OnGameOver)
	{
		SceneManager::GetInstance().SetActiveGameScene("GameOverScene");
	}
}

void GameOverComponent::OnSceneDeactivate()
{
	Locator::GetSoundSystem().StopMusic();
	Locator::GetSoundSystem().Play("win.wav",1.f,true);
}

