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

	GameObject* ChaseComponent::GetTarget() const
	{
		return target;
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

		if (path.empty() || GridPathLength(path, cellSize) > detectionRadius)
		{
			hasTargetCell = false;
			path.clear();
			Stop();
			return;
		}

		const float slack = cellSize * 0.2f;
		while (waypointIndex < path.size())
		{
			const Vector2Df center = GridCellCenter(path[waypointIndex], cellSize);
			if (std::abs(center.x - selfPosition.x) > slack || std::abs(center.y - selfPosition.y) > slack)
			{
				break;
			}
			++waypointIndex;
		}

		horizontalAxis = 0.f;
		verticalAxis = 0.f;
		if (waypointIndex >= path.size())
		{
			if (distanceSquared <= stopDistance * stopDistance)
			{
				return;
			}

			const float distance = std::sqrt(distanceSquared);
			horizontalAxis = offsetX / distance;
			verticalAxis = offsetY / distance;
			return;
		}

		const Vector2Df center = GridCellCenter(path[waypointIndex], cellSize);
		const float deltaX = center.x - selfPosition.x;
		const float deltaY = center.y - selfPosition.y;
		const bool offX = std::abs(deltaX) > slack;
		const bool offY = std::abs(deltaY) > slack;
		if (offX && offY)
		{
			if (std::abs(deltaX) <= std::abs(deltaY))
			{
				horizontalAxis = deltaX > 0.f ? 1.f : -1.f;
			}
			else
			{
				verticalAxis = deltaY > 0.f ? 1.f : -1.f;
			}
			return;
		}

		if (offX)
		{
			horizontalAxis = deltaX > 0.f ? 1.f : -1.f;
		}
		if (offY)
		{
			verticalAxis = deltaY > 0.f ? 1.f : -1.f;
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
