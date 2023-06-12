#include "PlayerComponent.h"
#include "AppComponents.h"
#include <memory>
#include "UpdateContext.h"
#include "InputSystem.h"
#include "EventManager.h"
#include "PlayerStatesInclude.h"

using namespace engine;

void PlayerComponent::Initialize()
{
	EventSubscribe(EventType::OnTriggerEnter);
	EventSubscribe(EventType::OnRespawn);

	GameObject* pPlayer = m_GameObject;

	SetSpawnPosition(pPlayer->m_Transform->GetWorldPosition());

	m_pAnimatedSprite = pPlayer->AddComponent<AnimatedSpriteComponent>();
	m_pAnimatedSprite->SetTexture("../Data/Sprites/burgertime-sprites.png");
	m_pAnimatedSprite->SetFrameTime(0.15f);
	m_pAnimatedSprite->SetFrameRange(0, 8);
	m_pAnimatedSprite->SetFlipTexture(engine::render::SDL_FLIP_NONE);
	m_pAnimatedSprite->SetSourceRect(engine::physics::Box({ 0,0 }, { 16, 16 }));
	m_pAnimatedSprite->SetDestinationRect(engine::physics::Box({ -8,-8 }, { 16,16 }));

	m_pStateManager = pPlayer->AddComponent<PlayerStateManagerComponent>();

	GameObject* m_pHudObject = m_GameObject->GetScene()->MakeGameObject(m_GameObject->GetTag() + "_lives");

	auto pLivesComponent = m_pHudObject->AddComponent<LivesComponent>();
	pLivesComponent->Linkplayer(m_GameObject);

	m_pHudObject->AddComponent<ScoreComponent>();

	auto go = m_GameObject->GetScene()->MakeGameObject();
	go->AddComponent<EnemyComponent>();
	m_Enemy = go;

	InitializeInputCommands();

}

void PlayerComponent::OnEvent(const Event& sent)
{
	if (sent.GetEventType() == EventType::OnTriggerEnter)
	{
		const OnTriggerEnterEvent& triggerEvent = reinterpret_cast<const OnTriggerEnterEvent&>(sent);
		if (triggerEvent.GetTrigger() == m_GameObject)
			if (triggerEvent.GetOther()->HasComponent<EnemyComponent>())
			{
				m_pStateManager->SwitchState(std::make_unique<DeadState>());
			}
	}
	else if (sent.GetEventType() == EventType::OnRespawn)
	{
		const OnRespawnEvent& respawnEvent = reinterpret_cast<const OnRespawnEvent&>(sent);
		if(respawnEvent.GetObject()->GetTag() == m_GameObject->GetTag())
		{
			m_GameObject->m_Transform->SetLocalPosition(m_SpawnPosition);
			m_pAnimatedSprite->Play();
			m_pStateManager->SwitchState(std::make_unique<WalkIdleState>());
		}
	}
}

void PlayerComponent::Update(const engine::UpdateContext& context)
{
	if (GetPlaterState() == PlayerStateEnum::Dead) return;

	glm::vec2 moveVec = m_MoveDirection * Settings::Speed * context.GetDeltaTime().ToFloat();
	*m_GameObject->m_Transform += glm::vec3(moveVec.x, moveVec.y, 0);

	m_MoveDirection = glm::vec2{ 0,0 };
}

void PlayerComponent::SetMoveDirection(glm::vec2 direction)
{
	m_MoveDirection += direction;

	if (glm::length(m_MoveDirection) > 1.0f)
	{
		m_MoveDirection = glm::normalize(m_MoveDirection);
	}
}

PlayerStateEnum PlayerComponent::GetPlaterState() const
{
	return m_pStateManager->GetState();
}

AnimatedSpriteComponent* PlayerComponent::GetAnimatedSpriteComponent() 
{
	return m_pAnimatedSprite;
}

void PlayerComponent::OnSceneActivate()
{
	using namespace engine;
	std::string soundPath = "main.wav";
	Locator::GetSoundSystem().Play(soundPath, 1.f, false);

}

void PlayerComponent::LateUpdate(const UpdateContext& context) 
{
	using namespace Input;
	InputSystem& inputSystem = Input::InputSystem::GetInstance();

	if (inputSystem.GetKeyboardState()->WasPressed(KeyboardButton::KEY_Z)) 
	{
		EventManager::GetInstance().SendEvent(OnTriggerEnterEvent{m_GameObject, m_Enemy});
	}

	float timestep = context.GetDeltaTime();

	const glm::vec2& currentPosition = m_GameObject->m_Transform->GetWorldPosition();

	m_MoveDelta = currentPosition - m_LastPosition;
	m_MoveDelta /= timestep;
	m_MoveDelta.y = -m_MoveDelta.y; // currently dow
	m_LastPosition = currentPosition;
	//APP_TRACE("{} {}", m_MoveDelta.x, m_MoveDelta.y);
	// m_MoveDelta will have length of around 40 (same as the player speed, at the time of coding this it is 40)

}


void PlayerComponent::InitializeInputCommands()
{
	//GameObject* go = m_GameObject;



	//// GAMEPAD ANALOG STICK
	//auto stick = gamepadDevice->GetGamepadState().GetAnalogStickFilteredPtr(false);
	//pInputCommandComponent->BindCommand(std::make_unique<MoveCommand>(go, stick), gamepadDevice, ControllerButton::THUMBSTICK_LEFT_MOVE, ButtonPressType::Hold);
}

