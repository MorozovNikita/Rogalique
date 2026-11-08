#include "pch.h"
#include "PhysicsSystem.h"

namespace Engine
{
	PhysicsSystem* PhysicsSystem::Instance()
	{
		static PhysicsSystem physicsSystem;
		return &physicsSystem;
	}

	float PhysicsSystem::GetFixedDeltaTime() const
	{
		return fixedDeltaTime;
	}

	void PhysicsSystem::Update()
	{
		for (size_t i = 0; i < colliders.size(); i++)
		{
			auto body = colliders[i]->GetGameObject()->GetComponent<RigidbodyComponent>();
			if (body->GetKinematic())
			{
				continue;
			}

			for (size_t j = 0; j < colliders.size(); j++)
			{
				if (j == i)
				{
					continue;
				}

				const auto intersection = colliders[i]->bounds.findIntersection(colliders[j]->bounds);
				if (intersection)
				{
					if (colliders[i]->isTrigger != colliders[j]->isTrigger)
					{
						if (triggersEnteredPair.find(colliders[i]) == triggersEnteredPair.end() && triggersEnteredPair.find(colliders[j]) == triggersEnteredPair.end())
						{
							Trigger trigger(colliders[i], colliders[j]);
							colliders[i]->OnTriggerEnter(trigger);
							colliders[j]->OnTriggerEnter(trigger);

							triggersEnteredPair.emplace(colliders[i], colliders[j]);
						}
					}
					else if (!colliders[i]->isTrigger)
					{
						float intersectionWidth = intersection->size.x;
						float intersectionHeight = intersection->size.y;
						Vector2Df intersectionPosition = { intersection->position.x - 0.5f * intersectionWidth, intersection->position.y - 0.5f * intersectionHeight };

						Vector2Df aPosition = { colliders[i]->bounds.position.x, colliders[i]->bounds.position.y };
						auto aTransform = colliders[i]->GetGameObject()->GetComponent<TransformComponent>();

						if (intersectionWidth > intersectionHeight)
						{
							if (intersectionPosition.y > aPosition.y)
							{
								aTransform->MoveBy({ 0, -intersectionHeight });
								std::cout << "Top collision" << std::endl;
							}
							else
							{
								aTransform->MoveBy({ 0, intersectionHeight });
								std::cout << "Down collision" << std::endl;
							}
						}
						else
						{
							if (intersectionPosition.x > aPosition.x)
							{
								aTransform->MoveBy({ -intersectionWidth, 0.f });
								std::cout << "Right collision" << std::endl;
							}
							else
							{
								aTransform->MoveBy({ intersectionWidth, 0.f });
								std::cout << "Left collision" << std::endl;
							}
						}

						Collision collision(colliders[i], colliders[j], *intersection);
						colliders[i]->OnCollision(collision);
						colliders[j]->OnCollision(collision);
					}
				}
			}
		}

		for (auto triggeredPair = triggersEnteredPair.cbegin(), nextTriggeredPair = triggeredPair; triggeredPair != triggersEnteredPair.cend(); triggeredPair = nextTriggeredPair)
		{
			++nextTriggeredPair;
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
		std::cout << "Subscribe " << collider << std::endl;
		colliders.push_back(collider);
	}
	void PhysicsSystem::Unsubscribe(ColliderComponent* collider)
	{
		std::cout << "Unsubscribe " << collider << std::endl;

		colliders.erase(std::remove_if(colliders.begin(), colliders.end(), [collider](ColliderComponent* obj) { return obj == collider; }), colliders.end());
	}
}