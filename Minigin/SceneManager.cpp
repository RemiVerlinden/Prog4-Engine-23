#include "SceneManager.h"
#include "Scene.h"
void dae::SceneManager::Update(const UpdateContext& context)
{
	if (m_pActiveScene != nullptr) m_pActiveScene->Update(context);
}

void dae::SceneManager::LateUpdate(const UpdateContext& context)
{
	if (m_pActiveScene != nullptr) m_pActiveScene->LateUpdate(context);
}

void dae::SceneManager::FixedUpdate(const UpdateContext& context)
{
	if (m_pActiveScene != nullptr) m_pActiveScene->FixedUpdate(context);
}

void dae::SceneManager::Render()
{
	if (m_pActiveScene) m_pActiveScene->Render();
}
void dae::SceneManager::Initialize()
{

}

const std::shared_ptr<dae::Scene> dae::SceneManager::AddGameScene(const std::string& name)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name));
	m_Scenes.push_back(scene);

	return scene;
}

const std::shared_ptr<dae::Scene> dae::SceneManager::GetGameScene(const std::string& sceneName)
{
	auto checkMatchingName = [sceneName](std::shared_ptr<Scene>& scene) { return scene->GetTag() == sceneName; };
	auto it = find_if(m_Scenes.begin(), m_Scenes.end(), checkMatchingName);

	if (it != m_Scenes.end())
	{
		return *it;
	}

	return nullptr;
}

void dae::SceneManager::RemoveGameScene(const std::string& name)
{
	auto checkMatchingName = [name](std::shared_ptr<Scene>& scene){ return scene->GetTag() == name; };
	auto it = find_if(m_Scenes.begin(), m_Scenes.end(), checkMatchingName);

	if (it != m_Scenes.end())
	{
		m_Scenes.erase(it);
	}
}

void dae::SceneManager::SetActiveGameScene(const std::string& name)
{
	auto checkMatchingName = [name](std::shared_ptr<Scene>& scene) { return scene->GetTag() == name; };
	auto it = find_if(m_Scenes.begin(), m_Scenes.end(), checkMatchingName);

	if (it != m_Scenes.end())
	{
		m_pNewActiveScene = *it;
	}
}

void dae::SceneManager::CheckNewActiveGameScene()
{
	if (m_pNewActiveScene != nullptr)
	{
		//Deactivate the current active scene
		if (m_pActiveScene != nullptr)
			m_pActiveScene->OnSceneDeactivated();

		//Set New Scene
		m_pActiveScene = m_pNewActiveScene;
		m_pNewActiveScene = nullptr;

		//Active the new scene and reset SceneTimer
		m_pActiveScene->OnSceneActivated();
	}
}

void dae::SceneManager::NextScene()
{
	for (size_t i = 0; i < m_Scenes.size(); ++i)
	{
		if (m_Scenes[i] == m_pActiveScene)
		{
			auto nextScene = (++i) % m_Scenes.size();
			m_pNewActiveScene = m_Scenes[nextScene];
			return;
		}
	}
}

void dae::SceneManager::PreviousScene()
{
	for (size_t i = 0; i < m_Scenes.size(); ++i)
	{
		if (m_Scenes[i] == m_pActiveScene)
		{
			if (i == 0) i = m_Scenes.size();
			--i;
			m_pNewActiveScene = m_Scenes[i];
			return;
		}
	}
}
