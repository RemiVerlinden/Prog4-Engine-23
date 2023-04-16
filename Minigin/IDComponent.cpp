#include "IDComponent.h"
#include "GameObject.h"

void dae::IDComponent::Clone(GameObject* clone)
{
	if (CanBeCloned() == false) return;
	clone->AddComponent<IDComponent>(GetComponentTag());
}
