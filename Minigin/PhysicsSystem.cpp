#include "PhysicsSystem.h"
#include "GameObject.h"

#include <cassert>

//bool dae::PhysicsSystem::CheckLevelCollision(GameObject* pObject) const
//{
//    assert(pObject && "GameObject pointer is null!");
//    std::vector<ColliderComponent*> colliders{pObject->GetAllComponents<ColliderComponent>()};
//    for (ColliderComponent* pCollider : colliders)
//    {
//        assert(pCollider && "ColliderComponent pointer is null!");
//        if (CheckLevelCollision(pCollider)) return true;
//    };
//
//    return false;
//}
//
//bool dae::PhysicsSystem::CheckLevelCollision(ColliderComponent* pCollider) const
//{
//    assert(pCollider && "ColliderComponent pointer is null!");
//    for (ColliderComponent* pOtherCollider : m_pColliders)
//    {
//        assert(pOtherCollider && "ColliderComponent in m_pColliders is null!");
//
//        const Box& a = pCollider->GetCollider();
//        const Box& b = pOtherCollider->GetCollider();
//
//        if (CheckCollisionAABB(a, b)) return true;
//    }
//    return false;
//}
//
//void dae::PhysicsSystem::AddCollider(ColliderComponent* pCollider)
//{
//    assert(pCollider && "ColliderComponent pointer is null!");
//    m_pColliders.emplace_back(pCollider);
//}
//
//void dae::PhysicsSystem::AddCollider(GameObject* pObject)
//{
//    assert(pObject && "GameObject pointer is null!");
//    std::vector<ColliderComponent*> colliders{pObject->GetAllComponents<ColliderComponent>()};
//    for (ColliderComponent* pCollider : colliders)
//    {
//        assert(pCollider && "ColliderComponent pointer is null!");
//        AddCollider(pCollider);
//    };
//}
//
//void dae::PhysicsSystem::RemoveCollider(ColliderComponent* pCollider)
//{
//    assert(pCollider && "ColliderComponent pointer is null!");
//    std::erase(m_pColliders, pCollider);
//}
//
//void dae::PhysicsSystem::RemoveCollider(GameObject* pObject)
//{
//    assert(pObject && "GameObject pointer is null!");
//    std::vector<ColliderComponent*> colliders{pObject->GetAllComponents<ColliderComponent>()};
//
//    for (ColliderComponent* pCollider : colliders)
//    {
//        assert(pCollider && "ColliderComponent pointer is null!");
//        RemoveCollider(pCollider);
//    }
//}

bool dae::PhysicsSystem::CheckCollisionAABB(const Box& a, const Box& b) const
{
    //The sides of the rectangles
    float leftA, leftB;
    float rightA, rightB;
    float topA, topB;
    float bottomA, bottomB;

    //Calculate the sides of rect A
    leftA = a.pos.x;
    rightA = a.pos.x + a.size.x;
    topA = a.pos.y;
    bottomA = a.pos.y + a.size.y;

    //Calculate the sides of rect B
    leftB = b.pos.x;
    rightB = b.pos.x + b.size.x;
    topB = b.pos.y;
    bottomB = b.pos.y + b.size.y;

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

struct Box
{
    glm::vec2 pos;
    glm::vec2 size;
};

struct Circle
{
    glm::vec2 pos;
    float radius;
};
std::pair<bool, glm::vec2> dae::PhysicsSystem::CheckCollisionCircleVsBox(const Circle& c, const Box& b) const
{
    // Calculate the nearest point on the rectangle to the circle
    glm::vec2 nearestPoint;
    nearestPoint.x = std::max(b.pos.x, std::min(c.pos.x, b.pos.x + b.size.x));
    nearestPoint.y = std::max(b.pos.y, std::min(c.pos.y, b.pos.y + b.size.y));

    // Create a vector from the circle to the nearest point
    glm::vec2 rayToNearest = nearestPoint - c.pos;

    float distance = glm::length(rayToNearest);

    if (distance == 0.0f) {
        // If distance is zero, meaning that the circle's center is on the rectangle's edge,
        // we could handle it as a special case, here we return no collision.
        return { false, c.pos };
    }

    // Calculate the magnitude of the vector using glm's length function
    float overlap = c.radius - distance;
    if (std::isnan(overlap)) overlap = 0;

    glm::vec2 newPosition = c.pos;
    // If the overlap is positive, a collision occurred
    bool overlapOccurred = overlap > 0;
    if (overlapOccurred)
    {
        // Calculate a new position that avoids overlap
        newPosition -= glm::normalize(rayToNearest) * overlap;
    }

    // Return both the collision status and the new position
    return { overlapOccurred, newPosition };
}


std::pair<bool, glm::vec2> dae::PhysicsSystem::CheckCollisionCircleVsBox(const Box& b, const Circle& c) const
{
    return CheckCollisionCircleVsBox(c,b);
}
