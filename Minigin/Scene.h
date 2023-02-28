#pragma once
#include "SceneManager.h"
#include "GameTime.h"

namespace dae
{
	class GameObject;
	class Scene final
	{
		friend const std::shared_ptr<dae::Scene> SceneManager::AddGameScene(const std::string& name);
	public:
		void Add(std::shared_ptr<GameObject> object);
		void Remove(std::shared_ptr<GameObject> object);
		void RemoveAll();

		void Update();
		void Render() const;

		//========================================================
		void LateUpdate();
		void FixedUpdate();

		std::string GetTag() { return m_Tag; };
		void OnSceneActivated() { m_GameTime.Start(); }
		void OnSceneDeactivated() { m_GameTime.Stop(); }

		GameTime* GetGameTime();
		//========================================================

		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;



	private: 
		explicit Scene(const std::string& name);


		std::string m_Tag;
		std::vector < std::shared_ptr<GameObject>> m_objects{};

		static unsigned int m_idCounter; 

		//========================================================

		GameTime m_GameTime;


		//========================================================
	};

}


