#include "Player.h"

#include "LocomotionAnimator.h"

#include <AttackComponent.h>
#include <ConeAreaComponent.h>
#include <MovementComponent.h>
#include <SpriteClipAnimatorComponent.h>
#include <SpriteColliderComponent.h>
#include <StatsComponent.h>

namespace Roguelike
{
Player::Player(const Engine::Vector2Df &position)
{
    gameObject = Engine::GameWorld::Instance()->CreateGameObject("Player");
    auto transform = gameObject->GetComponent<Engine::TransformComponent>();
    transform->SetWorldPosition(position);

    auto renderer = gameObject->AddComponent<Engine::SpriteRendererComponent>();
    auto animator = gameObject->AddComponent<Engine::SpriteClipAnimatorComponent>();
    renderer->SetOriginFraction(0.26f, 0.75f);
    animator->AddClip("idle", "knight_idle", 4, 1, 0.16f, true, -1);
    animator->AddClip("run", "knight_run", 7, 1, 0.08f, true, -1);
    animator->AddClip("attack", "knight_attack", 5, 1, 0.07f, false, 4);
    animator->Play("idle");
    renderer->SetPixelSize(160, 160);

    auto camera = gameObject->AddComponent<Engine::CameraComponent>();
    camera->SetWindow(&Engine::RenderSystem::Instance()->GetMainWindow());
    camera->SetBaseResolution(1280, 720);

    auto input = gameObject->AddComponent<Engine::InputComponent>();

    auto movement = gameObject->AddComponent<Engine::MovementComponent>();
    movement->SetSpeed(400.f);

    auto rigidbody = gameObject->AddComponent<Engine::RigidbodyComponent>();
    rigidbody->SetKinematic(false);

    auto collider = gameObject->AddComponent<Engine::SpriteColliderComponent>();
    collider->SetCollisionLayer(Engine::CollisionLayer::Actor);
    collider->SetCollisionMask(Engine::CollisionLayer::World);
    collider->SetBoundsScale(0.36f, 0.52f);

    auto states = gameObject->AddComponent<Engine::StatsComponent>(100.f, 50.f);

    auto cone = gameObject->AddComponent<Engine::ConeAreaComponent>();
    cone->SetRange(110.f);
    cone->SetAngle(70.f);

    auto locomotion = gameObject->AddComponent<LocomotionAnimator>();
    locomotion->SetMirrorHorizontal(true);
    auto attack = gameObject->AddComponent<Engine::AttackComponent>(3.f, Engine::AttackTrigger::Input);
    attack->SetAttackClip("attack");
}

Engine::GameObject *Player::GetGameObject()
{
    return gameObject;
}
} // namespace Roguelike
