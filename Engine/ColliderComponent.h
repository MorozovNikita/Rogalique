#pragma once

#include "Component.h"
#include "Collision.h"
#include "Trigger.h"
#include "PhysicsSystem.h"

#include <SFML/Graphics.hpp>

#include <vector>
#include <functional>

namespace Engine
{
	struct CollisionLayer
	{
		static constexpr unsigned int World = 1u << 0;
		static constexpr unsigned int Actor = 1u << 1;
		static constexpr unsigned int All = ~0u;
	};

	class ColliderComponent : public Component
	{
	public:
		ColliderComponent(GameObject* gameObject);

		virtual void Update(float deltaTime) = 0;
		virtual void Render() = 0;

		void SetTrigger(bool newIsTrigger);
		void SetCollisionLayer(unsigned int newLayer);
		void SetCollisionMask(unsigned int newMask);
		unsigned int GetCollisionLayer() const;
		unsigned int GetCollisionMask() const;

		void SubscribeCollision(std::function<void(Collision)> onCollisionAction);
		void UnsubscribeCollision(std::function<void(Collision)> onCollisionAction);

		void SubscribeTriggerEnter(std::function<void(Trigger)> onTriggerEnterAction);
		void UnsubscribeTriggerEnter(std::function<void(Trigger)> onTriggerEnterAction);

		void SubscribeTriggerExit(std::function<void(Trigger)> onTriggerExitAction);
		void UnsubscribeTriggerExit(std::function<void(Trigger)> onTriggerExitAction);

		friend class PhysicsSystem;

	protected:
		sf::FloatRect bounds;
		bool isTrigger = false;
		unsigned int layer = CollisionLayer::World;
		unsigned int mask = CollisionLayer::All;

		void OnCollision(Collision collision);
		void OnTriggerEnter(Trigger trigger);
		void OnTriggerExit(Trigger trigger);

		std::vector<std::function<void(Collision)>> onCollisionActions;
		std::vector<std::function<void(Trigger)>> onTriggerEnterActions;
		std::vector<std::function<void(Trigger)>> onTriggerExitActions;
	};
}