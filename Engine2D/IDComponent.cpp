#include "IDComponent.h"
#include "GameObject.h"

void engine::IDComponent::Clone(GameObject* clone)
{
	if (CanBeCloned() == false) return;
	clone->AddComponent<IDComponent>(GetComponentTag());
}
