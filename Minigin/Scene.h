#pragma once
#include "SceneManager.h"


namespace dae
{
	class UpdateContext;
	class GameObject;
	class Scene final
	{
		friend const std::shared_ptr<dae::Scene> SceneManager::AddGameScene(const std::string& name);
	public:
		void Add(std::shared_ptr<GameObject> object);
		void Remove(std::shared_ptr<GameObject> object);
		void RemoveAll();

		void Update(const UpdateContext& context);
		void Render() const;

		//========================================================
		void LateUpdate(const UpdateContext& context);
		void FixedUpdate(const UpdateContext& context);

		std::string GetTag() { return m_Tag; };
		void OnSceneActivated() {  }
		void OnSceneDeactivated() {  }

		//========================================================

		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;


	private: 
		explicit Scene(const std::string& name);
		void DeleteMarkedForDestroy();

		std::string m_Tag;
		std::vector <std::shared_ptr<GameObject>> m_objects{};

		static unsigned int m_idCounter; 

		//========================================================

		//========================================================
	};

}


