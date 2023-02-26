#pragma once


// what do we have here?
// what is a "Scene Factory" supposed to be????
// well well, I will be using the Scene factory file to create my Scenes
// which means I will be initializing all the SceneFactorys in here 
// hence the "Factory" part.
namespace dae
{
	namespace SceneFactory
	{
		void CreateScenes();

		void InitDefaultScene();
		void InitFpsDemoScene();
		void InitBonusScene();
	}
}


//class SceneFactory
//{
//public:
//	SceneFactory();
//	~SceneFactory() = default;
//	SceneFactory(const SceneFactory& other) = delete;
//	SceneFactory(SceneFactory&& other) = delete;
//	SceneFactory& operator=(const SceneFactory& other) = delete;
//	SceneFactory& operator=(SceneFactory&& other) = delete;
//
//private:
//
//};