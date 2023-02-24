#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "BaseComponent.h"
#include "GameTime.h"

dae::GameObject::~GameObject() = default;

void dae::GameObject::Update([[maybe_unused]]float ts)
{
	for (std::shared_ptr<BaseComponent> component : m_Components)
	{
		component->RootUpdate(ts);
	}
}

void dae::GameObject::LateUpdate([[maybe_unused]] float ts)
{
	for (std::shared_ptr<BaseComponent> component : m_Components)
	{
		component->RootLateUpdate(ts);
	}
}

void dae::GameObject::FixedUpdate([[maybe_unused]] float ts)
{
	for (std::shared_ptr<BaseComponent> component : m_Components)
	{
		component->RootFixedUpdate(ts);
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
