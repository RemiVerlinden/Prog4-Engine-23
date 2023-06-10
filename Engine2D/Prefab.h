#pragma once
#include <unordered_map>
#include <vector>
#include "GameObject.h"
#include <memory>

namespace engine
{
	class GameObject;
	class Scene;
	class Prefab
	{
	public:
		Prefab(const std::string& name);

		GameObject* Instantiate(Scene* pScene);

		GameObject* GetPrefabObject();
	private:

		std::unique_ptr<GameObject> m_PrefabObject;
	};
}

