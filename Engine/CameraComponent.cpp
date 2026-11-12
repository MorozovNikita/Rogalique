#include "pch.h"
#include "CameraComponent.h"
#include "TransformComponent.h"

#include <stdexcept>

namespace Engine
{
	CameraComponent::CameraComponent(GameObject* gameObject) : Component(gameObject)
	{
		view = new sf::View(sf::FloatRect({ 0, 0 }, { 800, -600 }));
		transform = gameObject->GetComponent<TransformComponent>();
	}

	CameraComponent::~CameraComponent()
	{
		delete view;
	}

	void CameraComponent::Update(float deltaTime)
	{
		if (window == nullptr)
		{
			throw std::runtime_error("Camera update without a window");
		}
		if (transform == nullptr)
		{
			throw std::runtime_error("Camera has no transform");
		}

		auto position = transform->GetWorldPosition();
		auto rotation = transform->GetWorldRotation();

		view->setCenter(Convert<sf::Vector2f, Vector2Df>(position));
		view->setRotation(sf::degrees(rotation));

		window->setView(*view);
	}

	void CameraComponent::Render()
	{
		if (window == nullptr)
		{
			throw std::runtime_error("Camera render without a window");
		}
	}

	void CameraComponent::SetBaseResolution(int width, int height)
	{
		view->setCenter({ width / 2.f, -height / 2.f });
		view->setSize({ static_cast<float>(width), -static_cast<float>(height) });
	}

	void CameraComponent::SetWindow(sf::RenderWindow* newWindow)
	{
		window = newWindow;
	}
	void CameraComponent::ZoomBy(float newZoom)
	{
		if (newZoom <= 0.f)
		{
			throw std::invalid_argument("Zoom must be greater than zero");
		}
		view->zoom(newZoom);
	}
}