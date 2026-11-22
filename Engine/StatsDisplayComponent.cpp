#include "pch.h"
#include "StatsDisplayComponent.h"

#include "StatsComponent.h"
#include "TransformComponent.h"

#include <stdexcept>
#include <utility>

namespace Engine
{
StatsDisplayComponent::StatsDisplayComponent(GameObject *gameObject) : Component(gameObject)
{
    stats = gameObject->GetComponent<StatsComponent>();
    transform = gameObject->GetComponent<TransformComponent>();
    if (stats == nullptr || transform == nullptr)
    {
        throw std::logic_error("Stats display requires stats and a transform");
    }
}

void StatsDisplayComponent::SetStrategy(std::unique_ptr<StatsDisplayStrategy> newStrategy)
{
    strategy = std::move(newStrategy);
}

void StatsDisplayComponent::SetOffset(const Vector2Df &newOffset)
{
    offset = newOffset;
}

void StatsDisplayComponent::Update(float)
{
}

void StatsDisplayComponent::Render()
{
    if (strategy == nullptr || stats == nullptr || transform == nullptr)
    {
        return;
    }

    const Vector2Df position = transform->GetWorldPosition();
    strategy->Render(*stats, {position.x + offset.x, position.y + offset.y});
}
} // namespace Engine
