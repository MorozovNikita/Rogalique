#include "DeathAnimator.h"

#include "GameWorld.h"
#include "MovementComponent.h"
#include "ResourceSystem.h"
#include "SpriteClipAnimatorComponent.h"
#include "StatsComponent.h"

#include <stdexcept>

namespace Roguelike
{
DeathAnimator::DeathAnimator(Engine::GameObject *gameObject) : Engine::Component(gameObject)
{
    stats = gameObject->GetComponent<Engine::StatsComponent>();
    animator = gameObject->GetComponent<Engine::SpriteClipAnimatorComponent>();
    movement = gameObject->GetComponent<Engine::MovementComponent>();
    if (stats == nullptr || animator == nullptr)
    {
        throw std::logic_error("Death animator requires stats and sprite clips");
    }
}

void DeathAnimator::SetSound(const std::string &key)
{
    soundKey = key;
}

void DeathAnimator::SetDestroyOnFinish(bool destroy)
{
    destroyOnFinish = destroy;
}

void DeathAnimator::Update(float)
{
    if (stats->GetCurrentHealth() > 0.f)
    {
        return;
    }

    if (!dying)
    {
        animator->Play("death", true);
        if (!soundKey.empty())
        {
            Engine::ResourceSystem::Instance()->PlayEffect(soundKey);
        }
        dying = true;
    }

    if (movement != nullptr)
    {
        movement->SetSpeed(0.f);
    }

    if (destroyOnFinish && animator->IsPlaying("death") && animator->IsFinished())
    {
        Engine::GameWorld::Instance()->DestroyGameObject(m_gameObject);
    }
}

void DeathAnimator::Render()
{
}
} // namespace Roguelike
