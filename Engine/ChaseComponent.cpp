#include "pch.h"
#include "ChaseComponent.h"

#include "GridPath.h"
#include "TransformComponent.h"

#include <cmath>

namespace Engine
{

	ChaseComponent::ChaseComponent(GameObject* gameObject)
		: DirectionComponent(gameObject)
	{
	}

	void ChaseComponent::Initialize(const ChaseSettings& settings)
	{
		target = settings.target;
		cellSize = settings.cellSize;
		walkable = settings.walkable;
		detectionRadius = settings.detectionRadius;
		stopDistance = settings.stopDistance;
		hasTargetCell = false;
		path.clear();
		waypointIndex = 0;
	}

	void ChaseComponent::Update(float deltaTime)
	{
		if (target == nullptr || cellSize == 0.f)
		{
			Stop();
			return;
		}

		const Vector2Df selfPosition = m_gameObject->GetComponent<TransformComponent>()->GetWorldPosition();
		const Vector2Df targetPosition = target->GetComponent<TransformComponent>()->GetWorldPosition();
		const float offsetX = targetPosition.x - selfPosition.x;
		const float offsetY = targetPosition.y - selfPosition.y;
		const float distanceSquared = offsetX * offsetX + offsetY * offsetY;

		if (distanceSquared > detectionRadius * detectionRadius)
		{
			hasTargetCell = false;
			path.clear();
			Stop();
			return;
		}

		const Cell selfCell = ToCell(selfPosition);
		const Cell targetCell = ToCell(targetPosition);

		if (selfCell == targetCell)
		{
			if (distanceSquared <= stopDistance * stopDistance)
			{
				Stop();
				return;
			}

			const float distance = std::sqrt(distanceSquared);
			horizontalAxis = offsetX / distance;
			verticalAxis = offsetY / distance;
			return;
		}

		if (!hasTargetCell || targetCell != lastTargetCell)
		{
			lastTargetCell = targetCell;
			hasTargetCell = true;
			path = FindPathBFS(walkable, selfCell, targetCell);
			waypointIndex = 0;
		}

		while (waypointIndex < path.size() && path[waypointIndex] == selfCell)
		{
			++waypointIndex;
		}

		horizontalAxis = 0.f;
		verticalAxis = 0.f;
		if (waypointIndex < path.size())
		{
			const Cell& next = path[waypointIndex];
			if (next.first > selfCell.first)
			{
				horizontalAxis = 1.f;
			}
			else if (next.first < selfCell.first)
			{
				horizontalAxis = -1.f;
			}

			if (next.second > selfCell.second)
			{
				verticalAxis = 1.f;
			}
			else if (next.second < selfCell.second)
			{
				verticalAxis = -1.f;
			}
		}
	}

	float ChaseComponent::GetHorizontalAxis() const
	{
		return horizontalAxis;
	}

	float ChaseComponent::GetVerticalAxis() const
	{
		return verticalAxis;
	}

	void ChaseComponent::Render()
	{
	}

	ChaseComponent::Cell ChaseComponent::ToCell(const Vector2Df& position) const
	{
		return
		{
			static_cast<int>(std::round(position.x / cellSize)),
			static_cast<int>(std::round(position.y / cellSize))
		};
	}

	void ChaseComponent::Stop()
	{
		horizontalAxis = 0.f;
		verticalAxis = 0.f;
	}

}
