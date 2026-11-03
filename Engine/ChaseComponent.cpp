#include "pch.h"
#include "ChaseComponent.h"

#include "GameWorld.h"
#include "TransformComponent.h"

#include <algorithm>
#include <cmath>
#include <map>
#include <queue>

namespace Engine
{

	ChaseComponent::ChaseComponent(GameObject* gameObject)
		: DirectionComponent(gameObject)
	{
		BuildWalkable();
	}

	void ChaseComponent::Update(float deltaTime)
	{
		if (player == nullptr)
		{
			player = GameWorld::Instance()->FindGameObject("Player");
		}
		if (player == nullptr)
		{
			horizontalAxis = 0.f;
			verticalAxis = 0.f;
			return;
		}

		const Cell selfCell = ToCell(m_gameObject->GetComponent<TransformComponent>()->GetWorldPosition());
		const Cell playerCell = ToCell(player->GetComponent<TransformComponent>()->GetWorldPosition());

		if (!hasPlayerCell || playerCell != lastPlayerCell)
		{
			lastPlayerCell = playerCell;
			hasPlayerCell = true;
			BuildPath(selfCell, playerCell);
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

	void ChaseComponent::BuildWalkable()
	{
		walkable.clear();

		const auto floors = GameWorld::Instance()->FindGameObjects("Floor");
		for (size_t i = 0; i < floors.size(); i++)
		{
			walkable.insert(ToCell(floors[i]->GetComponent<TransformComponent>()->GetWorldPosition()));
		}

		const auto walls = GameWorld::Instance()->FindGameObjects("Wall");
		for (size_t i = 0; i < walls.size(); i++)
		{
			walkable.erase(ToCell(walls[i]->GetComponent<TransformComponent>()->GetWorldPosition()));
		}
	}

	void ChaseComponent::BuildPath(const Cell& from, const Cell& to)
	{
		path.clear();
		waypointIndex = 0;

		if (walkable.find(from) == walkable.end() || walkable.find(to) == walkable.end())
		{
			return;
		}

		if (from == to)
		{
			path.push_back(from);
			return;
		}

		std::map<Cell, Cell> cameFrom;
		std::set<Cell> visited;
		std::queue<Cell> frontier;
		frontier.push(from);
		visited.insert(from);

		const int offsetX[4] = { 1, -1, 0, 0 };
		const int offsetY[4] = { 0, 0, 1, -1 };
		bool found = false;

		while (!frontier.empty())
		{
			const Cell current = frontier.front();
			frontier.pop();
			if (current == to)
			{
				found = true;
				break;
			}

			for (int i = 0; i < 4; i++)
			{
				const Cell next = { current.first + offsetX[i], current.second + offsetY[i] };
				if (walkable.find(next) == walkable.end() || visited.find(next) != visited.end())
				{
					continue;
				}

				visited.insert(next);
				cameFrom[next] = current;
				frontier.push(next);
			}
		}

		if (!found)
		{
			return;
		}

		for (Cell cell = to; cell != from; cell = cameFrom[cell])
		{
			path.push_back(cell);
		}
		path.push_back(from);
		std::reverse(path.begin(), path.end());
	}

	ChaseComponent::Cell ChaseComponent::ToCell(const Vector2Df& position) const
	{
		return
		{
			static_cast<int>(std::round(position.x / tileSize)),
			static_cast<int>(std::round(position.y / tileSize))
		};
	}

}
