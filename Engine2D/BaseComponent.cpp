#include "GameObject.h"
#include "BaseComponent.h"


engine::BaseComponent::BaseComponent(const std::string& name) 
	:m_ComponentTag(name)
{

}

void engine::BaseComponent::RootInitialize(GameObject* go)
{
	m_GameObject = go;
	Initialize();
}

void engine::BaseComponent::RootUpdate(const UpdateContext& context)
{
	Update(context);
}

void engine::BaseComponent::RootDraw()
{
	Draw();
}

void engine::BaseComponent::RootDrawUI(UpdateContext& context)
{
	DrawUI(context);
}

void engine::BaseComponent::RootFixedUpdate(const UpdateContext& context)
{
	FixedUpdate(context);
}

void engine::BaseComponent::RootLateUpdate(const UpdateContext& context)
{
	LateUpdate(context);
}

void engine::BaseComponent::RootOnSceneActivate()
{
	OnSceneActivate();
}

void engine::BaseComponent::RootOnSceneDeactivate()
{
	OnSceneDeactivate();
}

engine::GameObject* engine::BaseComponent::GetGameObject() const
{
	return m_GameObject;
}
