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

void dae::BaseComponent::RootUpdate(float ts)
{
	Update(ts);
}

void dae::BaseComponent::RootDraw()
{
	Draw();
}

void dae::BaseComponent::RootFixedUpdate(float ts)
{
	FixedUpdate(ts);
}

void dae::BaseComponent::RootLateUpdate(float ts)
{
	LateUpdate(ts);
}
