#include "Monster.h"

#include "DeathAnimator.h"
#include "LocomotionAnimator.h"

#include <AttackComponent.h>
#include <HealthArmorDisplayStrategy.h>
#include <StatsDisplayComponent.h>
#include <ChaseComponent.h>
#include <ConeAreaComponent.h>
#include <MovementComponent.h>
#include <SpriteClipAnimatorComponent.h>
#include <SpriteColliderComponent.h>
#include <StatsComponent.h>

#include <memory>

namespace Roguelike
{
Monster::Monster(const Engine::Vector2Df &position, const Engine::ChaseSettings &chaseSettings,
                 const MonsterAppearance &appearance)
{
    gameObject = Engine::GameWorld::Instance()->CreateGameObject("Vampire");
    auto transform = gameObject->GetComponent<Engine::TransformComponent>();
    transform->SetWorldPosition(position);

    auto renderer = gameObject->AddComponent<Engine::SpriteRendererComponent>();
    auto animator = gameObject->AddComponent<Engine::SpriteClipAnimatorComponent>();
    animator->AddClip("idle", appearance.idle, 4, 4, 0.16f, true, -1);
    animator->AddClip("run", appearance.run, 8, 4, 0.08f, true, -1);
    animator->AddClip("attack", appearance.attack, 12, 4, 0.07f, false, 7);
    animator->AddClip("death", appearance.death, 11, 4, 0.08f, false, -1);
    animator->Play("idle");
    renderer->SetPixelSize(160, 160);

    auto chase = gameObject->AddComponent<Engine::ChaseComponent>();
    chase->Initialize(chaseSettings);

    auto movement = gameObject->AddComponent<Engine::MovementComponent>();
    movement->SetSpeed(450.f);

    auto rigidbody = gameObject->AddComponent<Engine::RigidbodyComponent>();
    rigidbody->SetKinematic(false);

    auto collider = gameObject->AddComponent<Engine::SpriteColliderComponent>();
    collider->SetCollisionLayer(Engine::CollisionLayer::Actor);
    collider->SetCollisionMask(Engine::CollisionLayer::World);
    collider->SetBoundsScale(0.4f, 0.48f);

    gameObject->AddComponent<Engine::StatsComponent>(40.f, 0.f);

    auto cone = gameObject->AddComponent<Engine::ConeAreaComponent>();
    cone->SetRange(110.f);
    cone->SetAngle(70.f);

    gameObject->AddComponent<LocomotionAnimator>();
    auto attack = gameObject->AddComponent<Engine::AttackComponent>(60.f, Engine::AttackTrigger::Automatic);
    attack->SetAttackClip("attack");

    auto statsDisplay = gameObject->AddComponent<Engine::StatsDisplayComponent>();
    statsDisplay->SetStrategy(std::make_unique<Engine::HealthArmorDisplayStrategy>("bar_bg", "bar_hp", "bar_shield"));
    gameObject->AddComponent<DeathAnimator>();
}

Engine::GameObject *Monster::GetGameObject()
{
    return gameObject;
}
} // namespace Roguelike
