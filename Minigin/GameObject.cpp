#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "BaseComponent.h"
#include "GameTime.h"

dae::GameObject::GameObject(const std::string& tag) : m_Tag(tag)
{
};

dae::GameObject::~GameObject() = default;

void dae::GameObject::Update()
{
	for (std::shared_ptr<BaseComponent> component : m_Components)
	{
		component->RootUpdate();
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

void dae::GameObject::Initialize(GameTime* time)
{
	for (std::shared_ptr<BaseComponent> component : m_Components)
	{
		component->RootInitialize(this, time);
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
	m_transform.SetPosition(x, y, 0.0f);
}
