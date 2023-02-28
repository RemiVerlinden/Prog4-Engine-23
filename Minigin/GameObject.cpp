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
void dae::GameObject::SetParent([[maybe_unused]]GameObject* parent, [[maybe_unused]] bool keepWorldPosition)
{
	//if (parent == nullptr)
	//	SetLocalPosition(GetWorldPosition());
	//else
	//{
	//	if (keepWorldPosition)
	//		SetLocalPosition(GetLocalPosition() - parent->GetWorldPosition());
	//	SetPositionDirty()
	//}
	//if (m_parent)
	//	m_parent->RemoveChild(this);
	//m_parent = parent;
	//if (m_parent)
	//	m_parent->AddChild(this)
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
	m_transform.lock()->SetPosition(x, y, 0.0f);
}


