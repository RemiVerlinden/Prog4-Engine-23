#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "BaseComponent.h"
#include "GameTime.h"
#include "Transform.h"

dae::GameObject::GameObject(Scene* scene) :m_Scene(scene), m_Parent{nullptr}
{
	m_transform = AddComponent<TransformComponent>();
	m_Parent = nullptr;
};

dae::GameObject::~GameObject() = default;

void dae::GameObject::Update()
{
	for (std::shared_ptr<BaseComponent> component : m_Components)
	{
		component->RootUpdate();
	}
}
void dae::GameObject::SetParent(GameObject* parent, bool keepWorldPosition)
{
	std::shared_ptr<TransformComponent> transform = GetComponent<TransformComponent>().lock();
	std::shared_ptr<TransformComponent> parentTransform = parent->GetComponent<TransformComponent>().lock();
	if (parent == nullptr)
		transform->SetLocalPosition(transform->GetWorldPosition());
	else
	{
		if (keepWorldPosition)
			transform->SetLocalPosition(transform->GetLocalPosition() - parentTransform->GetWorldPosition());
		transform->SetPositionDirty();
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

void dae::GameObject::LateUpdate()
{
	for (std::shared_ptr<BaseComponent> component : m_Components)
	{
		component->RootLateUpdate();
	}
}

void dae::GameObject::FixedUpdate()
{
	for (std::shared_ptr<BaseComponent> component : m_Components)
	{
		component->RootFixedUpdate();
	}
}


void dae::GameObject::Render() const
{
	for (std::shared_ptr<BaseComponent> component : m_Components)
	{
		component->RootDraw();
	}
}

//void dae::GameObject::SetTexture(const std::string& filename)
//{
//	m_texture = ResourceManager::GetInstance().LoadTexture(filename);
//}
//
void dae::GameObject::SetPosition(float x, float y)
{
	m_transform.lock()->SetLocalPosition(x, y, 0.0f);
}


