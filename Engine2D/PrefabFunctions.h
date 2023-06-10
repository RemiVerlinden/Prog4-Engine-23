#pragma once
#include "SceneFactory.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Scene.h"
#include "TextComponent.h"
#include "FpsComponent.h"
#include "Render2DComponent.h"
#include "GameObject.h"
#include "OrbitComponent.h"
#include "InputSystem.h"
#include "InputDebugImguiComponent.h"
#include "TrashTheCacheComponent.h"

namespace engine
{
	namespace Prefab
	{
		void CreatePlayer(Scene* pScene)
		{


			std::shared_ptr<GameObject> go = pScene->MakeGameObject();

		}
	}
}