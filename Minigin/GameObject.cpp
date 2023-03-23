#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "BaseComponent.h"
#include "Scene.h"

#include "Transform.h"

using namespace dae;

uint64_t dae::GameObject::m_GameObjectCount{0};

dae::GameObject::GameObject(Scene* scene) :m_Scene(scene), m_Parent{ nullptr }, m_MarkedForDestroy{false}
{
	m_Transform = AddComponent<TransformComponent>();

	++m_GameObjectCount;
};

dae::GameObject::~GameObject() = default;

void dae::GameObject::Update(const UpdateContext& context)
{
	for (const std::unique_ptr<BaseComponent>& component : m_Components)
	{
		component->RootUpdate(context);
	}
	--m_GameObjectCount;
}
void dae::GameObject::SetParent(GameObject* parent, bool keepWorldPosition)
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

void dae::GameObject::AddChild(GameObject* go)
{
	m_Children.push_back(go);
}

void dae::GameObject::RemoveChild(GameObject* go)
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

void dae::GameObject::LateUpdate(const UpdateContext& context)
{
	for (const std::unique_ptr<BaseComponent>& component : m_Components)
	{
		component->RootLateUpdate(context);
	}
}

void dae::GameObject::FixedUpdate(const UpdateContext& context)
{
	for (const std::unique_ptr<BaseComponent>& component : m_Components)
	{
		component->RootFixedUpdate(context);
	}
}


void dae::GameObject::Render() const
{
	for (const std::unique_ptr<BaseComponent>& component : m_Components)
	{
		component->RootDraw();
	}
}

void dae::GameObject::RenderUI() const
{
	for (const std::unique_ptr<BaseComponent>& component : m_Components)
	{
		component->RootDrawUI();
	}
}

void dae::GameObject::SetPosition(float x, float y)
{
	m_Transform->SetLocalPosition(x, y, 0.0f);
}

void dae::GameObject::Destroy()
{
	m_MarkedForDestroy = true;
	SetParent(nullptr, true);
	for (const auto& child : m_Children)
	{
		child->Destroy();
	}
}

GameObject* dae::GameObject::GetChildAt(unsigned int index)
{
	assert(index >= 0 && index < m_Children.size());

	return m_Children.at(index);
}


