#include "BindInputCommandComponent.h"

dae::BindInputCommandComponent::~BindInputCommandComponent()
{
}

void dae::BindInputCommandComponent::Clone(GameObject* clone)
{
	if (CanBeCloned() == false) return;
		clone->AddComponent<BindInputCommandComponent>(GetComponentTag());
}
