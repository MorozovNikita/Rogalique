#pragma once

#include "DirectionComponent.h"
#include "Vector.h"

#include <set>
#include <utility>
#include <vector>

namespace Engine
{
	class GameObject;

	class ChaseComponent : public DirectionComponent
	{
	public:
		ChaseComponent(GameObject* gameObject);

		void Update(float deltaTime) override;
		void Render() override;

		float GetHorizontalAxis() const override;
		float GetVerticalAxis() const override;
	private:
		using Cell = std::pair<int, int>;

		void BuildWalkable();
		void BuildPath(const Cell& from, const Cell& to);
		Cell ToCell(const Vector2Df& position) const;

		GameObject* player = nullptr;

		float horizontalAxis = 0.f;
		float verticalAxis = 0.f;

		std::set<Cell> walkable;
		std::vector<Cell> path;

		size_t waypointIndex = 0;
		Cell lastPlayerCell = { -1, -1 };
		bool hasPlayerCell = false;

		float tileSize = 128.f;
	};
}
