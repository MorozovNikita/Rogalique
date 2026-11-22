#pragma once

#include "Component.h"
#include "Vector.h"

#include <memory>

namespace Engine
{
class StatsComponent;
class TransformComponent;

class StatsDisplayStrategy
{
  public:
    virtual ~StatsDisplayStrategy() = default;

    virtual void Render(const StatsComponent &stats, const Vector2Df &center) = 0;
};

class StatsDisplayComponent : public Component
{
  public:
    explicit StatsDisplayComponent(GameObject *gameObject);

    void SetStrategy(std::unique_ptr<StatsDisplayStrategy> newStrategy);
    void SetOffset(const Vector2Df &newOffset);

    void Update(float deltaTime) override;
    void Render() override;

  private:
    StatsComponent *stats = nullptr;
    TransformComponent *transform = nullptr;
    std::unique_ptr<StatsDisplayStrategy> strategy;
    Vector2Df offset{0.f, 96.f};
};
} // namespace Engine
