#include "BaseComponent.h"
#include "GameObject.h"
#include "GameTime.h"

dae::BaseComponent::BaseComponent()
{

}

void dae::BaseComponent::RootInitialize(GameObject* go, GameTime* time)
{
	m_GameObject = go;
	Initialize(time);
}

void dae::BaseComponent::RootUpdate()
{
	Update();
}

void dae::BaseComponent::RootDraw()
{
	Draw();
}

void dae::BaseComponent::RootFixedUpdate()
{
	FixedUpdate();
}

void dae::BaseComponent::RootLateUpdate()
{
	LateUpdate();
}
