#include "UpdateContext.h"
#include "CirclePhysicsComponent.h"
#include "GameObject.h"
#include "Locator.h"
#include "Transform.h"
#include "DebugRenderer.h"
#include "../BurgerTime/WorldPhysics.h"

void dae::CirclePhysicsComponent::Initialize()
{
	glm::vec3 objectPos = m_GameObject->m_Transform->GetWorldPosition();
	m_PhysicsCollider = Circle{ objectPos, 80.f };
}

void dae::CirclePhysicsComponent::Update(const UpdateContext& /*pContext*/)
{
	UpdateColliderPosition();

	Scene* pScene = m_GameObject->GetScene();
	pScene->GetPhysicsWorld().SolveWorldCollision(this);

	if(m_CollisionOccured)
		UpdateObjectPosition();

	
}

void dae::CirclePhysicsComponent::Draw() 
{
	glm::vec2 pos = m_PhysicsCollider.pos;
	float radius = m_PhysicsCollider.radius;
	DebugRenderer::GetInstance().RenderCircle((int)pos.x,(int)pos.y, (int)radius);

	Scene* pScene = m_GameObject->GetScene();
	WorldPhysics* worldPhysics = static_cast<WorldPhysics*>(&pScene->GetPhysicsWorld());
	worldPhysics->Draw();
}

void dae::CirclePhysicsComponent::UpdateColliderPosition() 
{
	glm::vec3 objectPos = m_GameObject->m_Transform->GetWorldPosition();
	m_PhysicsCollider.pos = objectPos;
}
void dae::CirclePhysicsComponent::UpdateObjectPosition()
{
	glm::vec2& newPos = m_PhysicsCollider.pos;
	m_GameObject->m_Transform->SetLocalPosition(newPos.x,newPos.y,0);
	m_CollisionOccured = false;
}
