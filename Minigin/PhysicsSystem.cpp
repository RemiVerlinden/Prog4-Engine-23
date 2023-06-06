#include "PhysicsSystem.h"
#include "GameObject.h"
#include "ColliderComponent.h"

bool dae::PhysicsSystem::CheckLevelCollision(GameObject* pObject) const
{
    std::vector<ColliderComponent*> colliders{pObject->GetAllComponents<ColliderComponent>()};
    for (ColliderComponent* pCollider : colliders)
    {
        if (CheckLevelCollision(pCollider)) return true;
    };

   return false;
}

bool dae::PhysicsSystem::CheckLevelCollision(ColliderComponent* pCollider) const
{
    for (ColliderComponent* pCollider : m_pColliders)
    {
        const Box& a = pCollider->GetCollider();
        const Box& b = pCollider->GetCollider();

        if (CheckCollisionAABB(a, b)) return true;
    }
    return false;
}

void dae::PhysicsSystem::AddCollider(ColliderComponent* pCollider)
{
    m_pColliders.emplace_back(pCollider);
}

void dae::PhysicsSystem::AddCollider(GameObject* pObject)
{
    std::vector<ColliderComponent*> colliders{pObject->GetAllComponents<ColliderComponent>()};
    for (ColliderComponent* pCollider : colliders)
    {
        AddCollider(pCollider);
    };
}

void dae::PhysicsSystem::RemoveCollider(ColliderComponent* pCollider)
{
    std::erase(m_pColliders, pCollider);
}

void dae::PhysicsSystem::RemoveCollider(GameObject* pObject)
{
    std::vector<ColliderComponent*> colliders{pObject->GetAllComponents<ColliderComponent>()};

    for (ColliderComponent* pCollider : colliders)
    {
        RemoveCollider(pCollider);
    }
}

bool dae::PhysicsSystem::CheckCollisionAABB(const Box& a, const Box& b) const
{
    //The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    //Calculate the sides of rect A
    leftA = a.x;
    rightA = a.x + a.w;
    topA = a.y;
    bottomA = a.y + a.h;

    //Calculate the sides of rect B
    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;

    //If any of the sides from A are outside of B
    if (bottomA <= topB)
    {
        return false;
    }

    if (topA >= bottomB)
    {
        return false;
    }

    if (rightA <= leftB)
    {
        return false;
    }

    if (leftA >= rightB)
    {
        return false;
    }

    //If none of the sides from A are outside B
    return true;
}