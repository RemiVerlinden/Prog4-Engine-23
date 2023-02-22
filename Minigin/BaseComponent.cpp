#include "BaseComponent.h"

void dae::BaseComponent::RootInitialize()
{
	Initialize();
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
