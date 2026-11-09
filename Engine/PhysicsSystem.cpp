#include "pch.h"
#include "PhysicsSystem.h"

#include "TransformComponent.h"

#include <algorithm>

namespace Engine
{
	namespace
	{
		bool physicsAlive = false;

		sf::Vector2f CenterOf(const sf::FloatRect& rect)
		{
			return { rect.position.x + rect.size.x * 0.5f, rect.position.y + rect.size.y * 0.5f };
		}

		bool IsKinematic(ColliderComponent* collider)
		{
			auto* body = collider->GetGameObject()->GetComponent<RigidbodyComponent>();
			return body == nullptr || body->GetKinematic();
		}

		bool CanInteract(const ColliderComponent* first, const ColliderComponent* second)
		{
			return (first->GetCollisionMask() & second->GetCollisionLayer()) != 0
				&& (second->GetCollisionMask() & first->GetCollisionLayer()) != 0;
		}

		void PushOut(ColliderComponent* body, const sf::FloatRect& bodyBounds, const sf::FloatRect& intersection, const sf::Vector2f& awayFrom, float share)
	{
		auto* transform = body->GetGameObject()->GetComponent<TransformComponent>();
		if (transform == nullptr || share == 0.f)
		{
			return;
		}

		const sf::Vector2f center = CenterOf(bodyBounds);
			const float offsetX = center.x - awayFrom.x;
			const float offsetY = center.y - awayFrom.y;

			if (intersection.size.x <= intersection.size.y)
			{
				const float sign = offsetX < 0.f ? -1.f : 1.f;
				transform->MoveBy({ sign * intersection.size.x * share, 0.f });
			}
			else
			{
				const float sign = offsetY < 0.f ? -1.f : 1.f;
				transform->MoveBy({ 0.f, sign * intersection.size.y * share });
			}

			body->Update(0.f);
		}
	}

	PhysicsSystem::PhysicsSystem()
	{
		physicsAlive = true;
	}

	PhysicsSystem::~PhysicsSystem()
	{
		physicsAlive = false;
		colliders.clear();
		triggersEnteredPair.clear();
	}

	PhysicsSystem* PhysicsSystem::Instance()
	{
		static PhysicsSystem physicsSystem;
		return &physicsSystem;
	}

	float PhysicsSystem::GetFixedDeltaTime() const
	{
		return fixedDeltaTime;
	}

	bool PhysicsSystem::IsAlive()
	{
		return physicsAlive;
	}

	void PhysicsSystem::Update()
	{
		if (!physicsAlive)
		{
			return;
		}

		for (ColliderComponent* collider : colliders)
		{
			if (collider != nullptr)
			{
				collider->Update(0.f);
			}
		}

		for (size_t i = 0; i < colliders.size(); i++)
		{
			ColliderComponent* first = colliders[i];
			if (first == nullptr)
			{
				continue;
			}

			for (size_t j = i + 1; j < colliders.size(); j++)
			{
				ColliderComponent* second = colliders[j];
				if (second == nullptr || !CanInteract(first, second))
				{
					continue;
				}

				const auto intersection = first->bounds.findIntersection(second->bounds);
				if (!intersection)
				{
					continue;
				}

				if (first->isTrigger != second->isTrigger)
				{
					bool alreadyEntered = false;
					for (const auto& triggeredPair : triggersEnteredPair)
					{
						const bool samePair = (triggeredPair.first == first && triggeredPair.second == second)
							|| (triggeredPair.first == second && triggeredPair.second == first);
						if (samePair)
						{
							alreadyEntered = true;
							break;
						}
					}

					if (!alreadyEntered)
					{
						Trigger trigger(first, second);
						first->OnTriggerEnter(trigger);
						second->OnTriggerEnter(trigger);
						triggersEnteredPair.emplace(first, second);
					}
					continue;
				}

				if (first->isTrigger)
				{
					continue;
				}

				const bool firstKinematic = IsKinematic(first);
				const bool secondKinematic = IsKinematic(second);
				if (firstKinematic && secondKinematic)
				{
					continue;
				}

				const sf::Vector2f firstCenter = CenterOf(first->bounds);
				const sf::Vector2f secondCenter = CenterOf(second->bounds);
				if (firstKinematic)
				{
					PushOut(second, second->bounds, *intersection, firstCenter, 1.f);
				}
				else if (secondKinematic)
				{
					PushOut(first, first->bounds, *intersection, secondCenter, 1.f);
				}
				else
				{
					const sf::FloatRect firstBounds = first->bounds;
					const sf::FloatRect secondBounds = second->bounds;
					PushOut(first, firstBounds, *intersection, secondCenter, 0.5f);
					PushOut(second, secondBounds, *intersection, firstCenter, 0.5f);
				}

				Collision collision(first, second, *intersection);
				first->OnCollision(collision);
				second->OnCollision(collision);
			}
		}

		for (auto triggeredPair = triggersEnteredPair.cbegin(), nextTriggeredPair = triggeredPair; triggeredPair != triggersEnteredPair.cend(); triggeredPair = nextTriggeredPair)
		{
			++nextTriggeredPair;
			if (triggeredPair->first == nullptr || triggeredPair->second == nullptr)
			{
				triggersEnteredPair.erase(triggeredPair);
				continue;
			}

			if (!triggeredPair->first->bounds.findIntersection(triggeredPair->second->bounds))
			{
				Trigger trigger(triggeredPair->first, triggeredPair->second);
				triggeredPair->first->OnTriggerExit(trigger);
				triggeredPair->second->OnTriggerExit(trigger);

				triggersEnteredPair.erase(triggeredPair);
			}
		}
	}

	void PhysicsSystem::Subscribe(ColliderComponent* collider)
	{
		if (!physicsAlive || collider == nullptr)
		{
			return;
		}

		colliders.push_back(collider);
	}

	void PhysicsSystem::Unsubscribe(ColliderComponent* collider)
	{
		if (!physicsAlive)
		{
			return;
		}

		colliders.erase(std::remove(colliders.begin(), colliders.end(), collider), colliders.end());

		for (auto triggeredPair = triggersEnteredPair.begin(); triggeredPair != triggersEnteredPair.end();)
		{
			if (triggeredPair->first == collider || triggeredPair->second == collider)
			{
				triggeredPair = triggersEnteredPair.erase(triggeredPair);
			}
			else
			{
				++triggeredPair;
			}
		}
	}
}
