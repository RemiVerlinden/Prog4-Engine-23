#include "BindInputCommandComponent.h"

engine::BindInputCommandComponent::~BindInputCommandComponent()
{
}

void engine::BindInputCommandComponent::Clone(GameObject* clone)
{
	if (CanBeCloned() == false) return;
		clone->AddComponent<BindInputCommandComponent>(GetComponentTag());
}
