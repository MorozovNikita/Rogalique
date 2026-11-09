#include "pch.h"
#include "GameWorld.h"

namespace Engine
{
	GameWorld* GameWorld::Instance()
	{
		static GameWorld world;
		return &world;
	}

	void GameWorld::Update(float deltaTime)
	{
		for (size_t i = 0; i < gameObjects.size(); i++)
		{
			gameObjects[i]->Update(deltaTime);
		}
	}

	GameWorld::~GameWorld()
	{
		Clear();
	}

	void GameWorld::FixedUpdate(float deltaTime)
	{
		const float fixedDeltaTime = PhysicsSystem::Instance()->GetFixedDeltaTime();
		const float maxFrameTime = fixedDeltaTime * 5.f;
		if (deltaTime > maxFrameTime)
		{
			deltaTime = maxFrameTime;
		}

		fixedCounter += deltaTime;
		while (fixedCounter >= fixedDeltaTime)
		{
			fixedCounter -= fixedDeltaTime;
			Update(fixedDeltaTime);
			PhysicsSystem::Instance()->Update();
		}
	}

	void GameWorld::Render()
	{
		for (size_t i = 0; i < gameObjects.size(); i++)
		{
			gameObjects[i]->Render();
		}
	}

	void GameWorld::LateUpdate()
	{
		std::vector<GameObject*> pending = std::move(markedToDestroyGameObjects);
		markedToDestroyGameObjects.clear();

		for (GameObject* gameObject : pending)
		{
			if (std::find(gameObjects.begin(), gameObjects.end(), gameObject) == gameObjects.end())
			{
				continue;
			}

			DestroyGameObjectImmediate(gameObject);
		}
	}

	GameObject* GameWorld::CreateGameObject()
	{
		GameObject* newGameObject = new GameObject();
		gameObjects.push_back(newGameObject);
		return newGameObject;
	}

	GameObject* GameWorld::CreateGameObject(std::string name)
	{
		GameObject* newGameObject = new GameObject(name);
		gameObjects.push_back(newGameObject);
		return newGameObject;
	}

	GameObject* GameWorld::FindGameObject(const std::string& name) const
	{
		for (size_t i = 0; i < gameObjects.size(); i++)
		{
			if (gameObjects[i] != nullptr && gameObjects[i]->GetName() == name)
			{
				return gameObjects[i];
			}
		}

		return nullptr;
	}

	std::vector<GameObject*> GameWorld::FindGameObjects(const std::string& name) const
	{
		std::vector<GameObject*> found;
		for (size_t i = 0; i < gameObjects.size(); i++)
		{
			if (gameObjects[i] != nullptr && gameObjects[i]->GetName() == name)
			{
				found.push_back(gameObjects[i]);
			}
		}

		return found;
	}

	void GameWorld::DestroyGameObject(GameObject* gameObject)
	{
		markedToDestroyGameObjects.push_back(gameObject);
	}

	void GameWorld::Clear()
	{
		std::vector<GameObject*> roots;
		for (GameObject* gameObject : gameObjects)
		{
			if (gameObject == nullptr)
			{
				continue;
			}

			auto* transform = gameObject->GetComponent<TransformComponent>();
			if (transform != nullptr && transform->GetParent() == nullptr)
			{
				roots.push_back(gameObject);
			}
		}

		for (GameObject* root : roots)
		{
			DestroyGameObjectImmediate(root);
		}

		gameObjects.clear();
		markedToDestroyGameObjects.clear();
		fixedCounter = 0.f;
	}

	void GameWorld::Print() const
	{
		for (auto& obj : gameObjects)
		{
			if (obj == nullptr)
			{
				continue;
			}
			if (obj->GetComponent<TransformComponent>()->GetParent() == nullptr)
			{
				obj->Print();
			}
		}
	}

	void GameWorld::DestroyGameObjectImmediate(GameObject* gameObject)
	{
		if (gameObject == nullptr || std::find(gameObjects.begin(), gameObjects.end(), gameObject) == gameObjects.end())
		{
			return;
		}

		auto* transform = gameObject->GetComponent<TransformComponent>();
		if (transform != nullptr)
		{
			auto* parent = transform->GetParent();
			if (parent != nullptr && parent->GetGameObject() != nullptr)
			{
				parent->GetGameObject()->RemoveChild(gameObject);
			}
		}

		std::vector<GameObject*> toDelete;
		for (auto* childTransform : gameObject->GetComponentsInChildren<TransformComponent>())
		{
			if (childTransform != nullptr && childTransform->GetGameObject() != nullptr)
			{
				toDelete.push_back(childTransform->GetGameObject());
			}
		}

		for (GameObject* gameObjectToDelete : toDelete)
		{
			gameObjects.erase(std::remove(gameObjects.begin(), gameObjects.end(), gameObjectToDelete), gameObjects.end());
			markedToDestroyGameObjects.erase(std::remove(markedToDestroyGameObjects.begin(), markedToDestroyGameObjects.end(), gameObjectToDelete), markedToDestroyGameObjects.end());
		}

		for (auto gameObjectToDelete = toDelete.rbegin(); gameObjectToDelete != toDelete.rend(); ++gameObjectToDelete)
		{
			delete *gameObjectToDelete;
		}
	}
}