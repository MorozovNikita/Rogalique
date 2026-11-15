#pragma once

#include "DirectionComponent.h"
#include "Vector.h"

#include <set>
#include <utility>
#include <vector>

namespace Engine
{
	class GameObject;

	struct ChaseSettings
	{
		GameObject* target = nullptr;
		float cellSize = 0.f;
		std::set<std::pair<int, int>> walkable;
		float detectionRadius = 0.f;
		float stopDistance = 0.f;
	};

	class ChaseComponent : public DirectionComponent
	{
	public:
		using Cell = std::pair<int, int>;

		ChaseComponent(GameObject* gameObject);

		void Initialize(const ChaseSettings& settings);
		GameObject* GetTarget() const;

		void Update(float deltaTime) override;
		void Render() override;

		float GetHorizontalAxis() const override;
		float GetVerticalAxis() const override;
	private:
		Cell ToCell(const Vector2Df& position) const;
		void Stop();

		GameObject* target = nullptr;

		float horizontalAxis = 0.f;
		float verticalAxis = 0.f;

		std::set<Cell> walkable;
		std::vector<Cell> path;

		size_t waypointIndex = 0;
		Cell lastTargetCell = { -1, -1 };
		bool hasTargetCell = false;

		float cellSize = 0.f;
		float detectionRadius = 0.f;
		float stopDistance = 0.f;
	};
}
