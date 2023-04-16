//#pragma once
//
//#include <unordered_map>
//#include "BaseComponent.h"
//#include <functional>
//#include "GameObject.h"
//namespace dae
//{
//	class SpawnerObject final
//	{
//	private:
//		// I dont want to store real actual components because they sometimes create other components upon initialization
//		// so I have to find a way to store something that could create a component when it is needed
//		// hence I store a function pointer that can create a component when required
//		using ComponentCreatorMap = std::unordered_map<std::string, std::function<BaseComponent*()>>;
//
//	public:
//		template <typename Component>
//		void addComponent(const std::string& name)
//		{
//			m_Components[name] = [](GameObject* go) { return go->AddComponent<Component>(name); };
//		}
//
//		void removeComponent(const std::string& name)
//		{
//			m_Components.erase(name);
//		}
//
//		ComponentCreatorMap& GetComponentMap() { return m_Components; }
//	private:
//		ComponentCreatorMap m_Components;
//	};
//}