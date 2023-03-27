#include "GameObject.h"
#include "BaseComponent.h"


dae::BaseComponent::BaseComponent() 
{

}

void dae::BaseComponent::RootInitialize(GameObject* go)
{
	m_GameObject = go;
	Initialize();
}

void dae::BaseComponent::RootUpdate(const UpdateContext& context)
{
	Update(context);
}

void dae::BaseComponent::RootDraw()
{
	Draw();
}

void dae::BaseComponent::RootDrawUI(UpdateContext& context)
{
	DrawUI(context);
}

void dae::BaseComponent::RootFixedUpdate(const UpdateContext& context)
{
	FixedUpdate(context);
}

void dae::BaseComponent::RootLateUpdate(const UpdateContext& context)
{
	LateUpdate(context);
}
