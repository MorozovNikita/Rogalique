#include "pch.h"
#include "Component.h"

namespace Engine
{

	Component::Component(GameObject* gameObject) : m_gameObject(gameObject)
	{
	}

	Component::~Component()
	{
	}

	GameObject* Component::GetGameObject()
	{
		return m_gameObject;
	}

}