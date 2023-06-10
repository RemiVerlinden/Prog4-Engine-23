#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "BaseComponent.h"

#include "Transform.h"
#include "IDComponent.h"
#include "TagComponent.h"

using namespace engine;


engine::GameObject::GameObject(std::string name) : GameObject(nullptr, name){}


engine::GameObject::GameObject(Scene* scene, std::string name) :m_Scene(scene), m_Parent{ nullptr }, m_MarkedForDestroy{ false }
{
	m_Transform = AddComponent<TransformComponent>();
	m_Transform->SetCanBeCloned(false);
	AddComponent<IDComponent>()->SetCanBeCloned(false);
	auto tagComponent = AddComponent<TagComponent>();
	tagComponent->SetCanBeCloned(false);

	tagComponent->m_Tag = name;
	m_Tag = name;

	++m_GameObjectCount;
};

engine::GameObject::~GameObject()
{
	--m_GameObjectCount;
}

void engine::GameObject::Update(const UpdateContext& context)
{
	for (const std::unique_ptr<BaseComponent>& component : m_Components)
	{
		component->RootUpdate(context);
	}
	--m_GameObjectCount;
}

void engine::GameObject::SetParent(GameObject* parent, bool keepWorldPosition)
{
	if (parent == nullptr)
		m_Transform->SetLocalPosition(m_Transform->GetWorldPosition());
	else
	{
		if (keepWorldPosition)
		{
			TransformComponent* parentTransform = parent->GetComponent<TransformComponent>();
			m_Transform->SetLocalPosition(m_Transform->GetLocalPosition() - parentTransform->GetWorldPosition());
		}
		m_Transform->SetPositionDirty();
	}
	if (m_Parent)
		m_Parent->RemoveChild(this);
	m_Parent = parent;
	if (m_Parent)
		m_Parent->AddChild(this);
}

void engine::GameObject::AddChild(GameObject* go)
{
	m_Children.push_back(go);
}

void engine::GameObject::RemoveChild(GameObject* go)
{
	for (auto itChild = m_Children.begin(); itChild != m_Children.end(); ++itChild)
	{
		if (*itChild == go)
		{
			m_Children.erase(itChild);
			break;
		}
	}
}

void engine::GameObject::LateUpdate(const UpdateContext& context)
{
	for (const std::unique_ptr<BaseComponent>& component : m_Components)
	{
		component->RootLateUpdate(context);
	}
}

void engine::GameObject::FixedUpdate(const UpdateContext& context)
{
	for (const std::unique_ptr<BaseComponent>& component : m_Components)
	{
		component->RootFixedUpdate(context);
	}
}


void engine::GameObject::Render() const
{
	for (const std::unique_ptr<BaseComponent>& component : m_Components)
	{
		component->RootDraw();
	}
}

void engine::GameObject::RenderUI(UpdateContext& context) const
{
	for (const std::unique_ptr<BaseComponent>& component : m_Components)
	{
		component->RootDrawUI(context);
	}
}

void engine::GameObject::OnSceneActivate()
{
	for (const std::unique_ptr<BaseComponent>& component : m_Components)
	{
		component->RootOnSceneActivate();
	}
}

void engine::GameObject::OnSceneDeactivate()
{
	for (const std::unique_ptr<BaseComponent>& component : m_Components)
	{
		component->RootOnSceneDeactivate();
	}
}

void engine::GameObject::SetPosition(float x, float y)
{
	m_Transform->SetLocalPosition(x, y, 0.0f);
}

void engine::GameObject::Destroy()
{
	m_MarkedForDestroy = true;
	SetParent(nullptr, true);
	for (const auto& child : m_Children)
	{
		child->Destroy();
	}
}

GameObject* engine::GameObject::GetChildAt(unsigned int index)
{
	assert(index >= 0 && index < m_Children.size());

	return m_Children.at(index);
}


