#include "pch.h"
#include "SceneManager.h"
#include "Scene/Scene.h"
#include "Scene/DevScene.h"
#include "Scene/EditScene.h"
#include "Scene/PlayScene.h"
#include "LevelData/LevelData.h"
#include "DataManager.h"

void SceneManager::init()
{
	ChangeScene(SceneType::DEVSCENE);
}

void SceneManager::Update()
{
	if (_scene)
		_scene->Update();
}

void SceneManager::Render(ID2D1RenderTarget* renderTarget, float alpha)
{
	if (_scene)
		_scene->Render(renderTarget, alpha);
}

void SceneManager::Clear()
{
	SAFE_DELETE(_scene);
}

void SceneManager::ChangeScene(SceneType sceneType)
{
	//if (_sceneType == sceneType)
		//return;

	ChangeScene(sceneType, L"");

}

void SceneManager::ChangeScene(SceneType sceneType, const std::wstring& mapFilePath)
{
	Scene* newScene = nullptr;

	switch (sceneType)
	{
	case SceneType::DEVSCENE:
		newScene = new DevScene();
		break;
	case SceneType::EDITSCENE:
		newScene = new EditScene();
		break;
	case SceneType::PLAYSCENE:
		newScene = new PlayScene();
		break;
	}

	if (_scene != nullptr) _scene->Clear();
	SAFE_DELETE(_scene);

	_scene = newScene;
	_sceneType = sceneType;

	

	if (_scene != nullptr && !mapFilePath.empty())
	{
		GET_SINGLE(DataManager)->LoadMapData(mapFilePath);
		MapData loadedMapData = GET_SINGLE(DataManager)->GetCurrentMapData();
		_scene->BuildMapFromData(loadedMapData);
	}

	if (_scene)
	{
		_scene->Init();
	}
}

void SceneManager::ChangeMap(const MapData& nextMapData)
{
	if (_scene == nullptr) return;

	//WPTYPE currentWP =  //TODO

	_scene->Clear();
	_scene->BuildMapFromData(nextMapData);
}

void SceneManager::ResetCurrentMap()
{
	if (_scene == nullptr) return;

	_scene->Clear();
	MapData loadedMapData = GET_SINGLE(DataManager)->GetCurrentMapData();

	_scene->BuildMapFromData(loadedMapData);
	_scene->Init();
}




void SceneManager::SetCamOwner(GameObject* object)
{
	_scene->SetCamOwner(object);
}

Vec2F SceneManager::ToRenderPos(Vec2F alphaPos)
{
	Vec2F renderPos = alphaPos - cameraPos + halfWinSizeV;
	return renderPos;
}

