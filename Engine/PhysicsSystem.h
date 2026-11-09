#pragma once

#include "ColliderComponent.h"
#include "RigidbodyComponent.h"
#include "Vector.h"

#include <map>

namespace Engine
{
	class PhysicsSystem
	{
	public:
		static PhysicsSystem* Instance();

		void Update();

		float GetFixedDeltaTime() const;
		static bool IsAlive();
		void Subscribe(ColliderComponent* collider);
		void Unsubscribe(ColliderComponent* collider);
	private:
		PhysicsSystem();
		~PhysicsSystem();

		PhysicsSystem(PhysicsSystem const&) = delete;
		PhysicsSystem& operator= (PhysicsSystem const&) = delete;

		std::vector<ColliderComponent*> colliders;
		std::map<ColliderComponent*, ColliderComponent*> triggersEnteredPair;

		float fixedDeltaTime = 0.02f;
	};
}