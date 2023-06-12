#include "UpdateContext.h"
#include "CirclePhysicsComponent.h"
#include "GameObject.h"
#include "Locator.h"
#include "Transform.h"
#include "DebugRenderer.h"
#include "../BurgerTime/WorldPhysics.h"
#include "Structs.h"

using namespace engine::physics;

void engine::CirclePhysicsComponent::Initialize()
{
	glm::vec3 objectPos = m_GameObject->m_Transform->GetWorldPosition();
	m_PhysicsCollider = Circle{ objectPos, 8.f };

	if (!m_CanLeaveWindow)
		ClampColliderPosToWindow();
}

void engine::CirclePhysicsComponent::Update(const UpdateContext& /*pContext*/)
{
	UpdateColliderPosition();

	Scene* pScene = m_GameObject->GetScene();
	pScene->GetPhysicsWorld().SolveWorldCollision(this);

	if(m_CollisionOccured)
		UpdateObjectPosition();
}

void engine::CirclePhysicsComponent::Draw() 
{
	//glm::vec2 pos = m_PhysicsCollider.pos;
	//float radius = m_PhysicsCollider.radius;
	//DebugRenderer::GetInstance().RenderCircle((int)pos.x,(int)pos.y, (int)radius);

	//Scene* pScene = m_GameObject->GetScene();
	//WorldPhysics* worldPhysics = static_cast<WorldPhysics*>(&pScene->GetPhysicsWorld());
	//worldPhysics->Draw();
}

void engine::CirclePhysicsComponent::UpdateColliderPosition() 
{
	glm::vec3 objectPos = m_GameObject->m_Transform->GetWorldPosition();
	m_PhysicsCollider.pos = objectPos;

	if (!m_CanLeaveWindow)
		ClampColliderPosToWindow();
	//APP_TRACE("player: {}, {}", objectPos.x, objectPos.y);
}
void engine::CirclePhysicsComponent::UpdateObjectPosition()
{
	glm::vec2& newPos = m_PhysicsCollider.pos;
	m_GameObject->m_Transform->SetLocalPosition(newPos.x,newPos.y,0);
	m_CollisionOccured = false;
}

void engine::CirclePhysicsComponent::ClampColliderPosToWindow() 
{
	glm::vec2& pos = m_PhysicsCollider.pos;
	float radius = m_PhysicsCollider.radius;
	pos.x = std::clamp(pos.x, radius, WindowSettings::width - radius);
	pos.y = std::clamp(pos.y, radius, WindowSettings::height - radius);

	m_CollisionOccured = true;
}
