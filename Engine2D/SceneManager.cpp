#include "SceneManager.h"
#include "Scene.h"
void engine::SceneManager::Update(const UpdateContext& context)
{
	if (m_pActiveScene != nullptr) m_pActiveScene->Update(context);
}

void engine::SceneManager::LateUpdate(const UpdateContext& context)
{
	if (m_pActiveScene != nullptr) m_pActiveScene->LateUpdate(context);
}

void engine::SceneManager::FixedUpdate(const UpdateContext& context)
{
	if (m_pActiveScene != nullptr) m_pActiveScene->FixedUpdate(context);
}

void engine::SceneManager::Render()
{
	if (m_pActiveScene) m_pActiveScene->Render();
}

void engine::SceneManager::RenderUI(UpdateContext& context)
{
	if (m_pActiveScene) m_pActiveScene->RenderUI(context);
}
void engine::SceneManager::Initialize()
{

}

engine::Scene* engine::SceneManager::AddGameScene(const std::string& name)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name));

	Scene* pScene = scene.get();
	m_Scenes.push_back(std::move(scene));

	return pScene;
}

engine::Scene* engine::SceneManager::GetGameScene(const std::string& sceneName)
{
	auto checkMatchingName = [sceneName](std::shared_ptr<Scene>& scene) { return scene->GetTag() == sceneName; };
	auto it = find_if(m_Scenes.begin(), m_Scenes.end(), checkMatchingName);

	if (it != m_Scenes.end())
	{
		return it->get();
	}

	return nullptr;
}

void engine::SceneManager::RemoveGameScene(const std::string& name)
{
	auto checkMatchingName = [name](std::shared_ptr<Scene>& scene){ return scene->GetTag() == name; };
	auto it = find_if(m_Scenes.begin(), m_Scenes.end(), checkMatchingName);

	if (it != m_Scenes.end())
	{
		m_Scenes.erase(it);
	}
}

void engine::SceneManager::SetActiveGameScene(const std::string& name)
{
	auto checkMatchingName = [name](std::shared_ptr<Scene>& scene) { return scene->GetTag() == name; };
	auto it = find_if(m_Scenes.begin(), m_Scenes.end(), checkMatchingName);

	if (it != m_Scenes.end())
	{
		m_pNewActiveScene = it->get();
	}
}

void engine::SceneManager::SetActiveGameScene(Scene* pScene)
{
	auto checkMatchingName = [pScene](std::shared_ptr<Scene>& scene) { return scene.get() == pScene; };
	auto it = find_if(m_Scenes.begin(), m_Scenes.end(), checkMatchingName);

	if (it != m_Scenes.end())
	{
		m_pNewActiveScene = it->get();
	}
}




engine::Scene* engine::SceneManager::GetActiveGameScene()
{
	return m_pActiveScene;
}

void engine::SceneManager::CheckNewActiveGameScene()
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

void engine::SceneManager::NextScene()
{
	
	for (size_t i = 0; i < m_Scenes.size(); ++i)
	{
		if (m_Scenes[i].get() == m_pActiveScene)
		{
			auto nextScene = (++i) % m_Scenes.size();
			m_pNewActiveScene = m_Scenes[nextScene].get();
			return;
		}
	}
}

void engine::SceneManager::PreviousScene()
{
	for (size_t i = 0; i < m_Scenes.size(); ++i)
	{
		if (m_Scenes[i].get() == m_pActiveScene)
		{
			if (i == 0) i = m_Scenes.size();
			--i;
			m_pNewActiveScene = m_Scenes[i].get();
			return;
		}
	}
}
