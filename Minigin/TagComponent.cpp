#include "TagComponent.h"
#include "GameObject.h"

void dae::TagComponent::Clone(GameObject* clone)
{
	if (CanBeCloned() == false) return;
	auto componentClone = clone->AddComponent<TagComponent>(GetComponentTag());
	componentClone->m_Tag = m_Tag;
}
