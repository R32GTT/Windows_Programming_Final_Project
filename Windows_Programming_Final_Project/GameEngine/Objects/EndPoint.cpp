#include "pch.h"
#include "EndPoint.h"
#include "../LevelData/LevelData.h"
#include "Managers.h"
#include "../Scene/Scene.h"

EndPoint::EndPoint()
{
	type = OBJECTTYPE::ENDPOINT;
	layer = Layers::WALL;
}

EndPoint::~EndPoint()
{
}

void EndPoint::OnCollision(GameObject* other)
{
	if (other->GetObjectType() == OBJECTTYPE::PLAYER)
	{
		Scene* currentScene = GET_SINGLE(SceneManager)->GetCurrentScene();

		if (currentScene != nullptr && currentScene->CheckSceneCleared())
		{
			GET_SINGLE(SceneManager)->RequestMapChange(_targetMapName);
		}
		else
		{
			// 클리어되지 않았을 때의 처리 (선택 사항)
			// 예: 화면에 "적을 모두 제거하십시오" 텍스트 띄우기
		}
	}
}

void EndPoint::SaveToData(ObjectSpawnData& outData)
{
	GameObject::SaveToData(outData);
	outData.textData = _targetMapName;
}

void EndPoint::LoadFromData(const ObjectSpawnData& spawnData)
{
	GameObject::LoadFromData(spawnData);
	_targetMapName = spawnData.textData;
}
