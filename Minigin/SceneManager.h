#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Singleton.h"

namespace dae
{
	class UpdateContext;
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene(const std::string& name);

		//=========================================================
		Scene* AddGameScene(const std::string& name);
		Scene* GetGameScene(const std::string& name);
		void RemoveGameScene(const std::string& name);
		void SetActiveGameScene(const std::string& name);
		void CheckNewActiveGameScene();

		void NextScene();
		void PreviousScene();

		void LateUpdate(const UpdateContext& context);
		void FixedUpdate(const UpdateContext& context);
		void Initialize();
		//=========================================================
		void Update(const UpdateContext& context);
		void Render();
		void RenderUI(UpdateContext& context);
	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		std::vector<std::shared_ptr<Scene>> m_Scenes;


		//=========================================================
		Scene* m_pActiveScene;
		Scene* m_pNewActiveScene;
		//=========================================================
	};
}
