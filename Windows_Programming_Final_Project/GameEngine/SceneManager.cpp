#include "pch.h"
#include "SceneManager.h"
#include "Scene/Scene.h"
#include "Scene/DevScene.h"
#include "Scene/EditScene.h"
#include "Scene/PlayScene.h"
#include "LevelData/LevelData.h"
#include "DataManager.h"
#include "TimeManager.h"

void SceneManager::init()
{
	//GET_SINGLE(DataManager)->LoadChaperData(L"Chapter1.json");
}

void SceneManager::Update()
{
	float dt = GET_SINGLE(TimeManager)->GetDeltaTime();

	if (!_isGameEnded)
	{
		if (_totalScore > 0.f)
		{
			_totalScore -= SCORE_TIME_PENALTY * dt;
			if (_totalScore < 0.f) _totalScore = 0.f;
		}

		if (_currentCombo > 0)
		{
			_comboTimer -= dt;
			if (_comboTimer <= 0.f)
			{
				_currentCombo = 0;
			}

		}
	}

	if (_scene) _scene->Update();

	if (_isMapChangeRequested)
	{
		ExecuteMapChange();
	}

}

void SceneManager::Render(ID2D1RenderTarget* renderTarget, float alpha)
{
	//현재 씬을 먼저 그립니다
	if (_scene)
		_scene->Render(renderTarget, alpha);

}

void SceneManager::Clear()
{
	SAFE_DELETE(_scene);
}

void SceneManager::ChangeScene(SceneType sceneType)
{
	

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

void SceneManager::ChangeScene(SceneType sceneType, const std::wstring& mapFilePath, const std::wstring& chapterFilePath)
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
	if (!chapterFilePath.empty())
	{
		GET_SINGLE(DataManager)->LoadChaperData(chapterFilePath);
	}

	if (_scene != nullptr && !mapFilePath.empty())
	{
		GET_SINGLE(DataManager)->LoadMapData(mapFilePath);
		MapData loadedMapData = GET_SINGLE(DataManager)->GetCurrentMapData();
		_scene->BuildMapFromData(loadedMapData);
	}
	else if (_scene != nullptr && !chapterFilePath.empty())
	{
		MapData chapterFirstMap = GET_SINGLE(DataManager)->GetCurrentMapData();
		_scene->BuildMapFromData(chapterFirstMap);
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

void SceneManager::RequestMapChange(const std::string& targetMapName)
{
	_isMapChangeRequested = true;
	_nextMapName = targetMapName;
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

void SceneManager::ExecuteMapChange()
{

	_isMapChangeRequested = false;

	_currentCombo = 0;
	_comboTimer = 0.f;
	_savedScore = _totalScore;

	bool hasNextMap = GET_SINGLE(DataManager)->GoToNextMap(_nextMapName);

	if (hasNextMap)
	{
		//ChangeMap(GET_SINGLE(DataManager)->GetCurrentMapData()); // 왜 있는거야 도데체
	}
	else
	{
		bool hasNextChapter = GET_SINGLE(DataManager)->GoToNextChapter();
		if (hasNextChapter)
		{
			ChangeMap(GET_SINGLE(DataManager)->GetCurrentMapData());
		}
		else
		{
			GET_SINGLE(DataManager)->EndGame();

			_isGameEnded = true;
		}
	}
}

//점수 처리를 위한 함수 구현
void SceneManager::OnEnemyKilled(WPTYPE wptype)
{
	if (_isGameEnded) return;

	// 무기 타입에 따른 점수 계산 로직 추가
	int scoreToAdd = 0;

	switch (wptype)
	{
	case WPTYPE::CROWBAR:
		scoreToAdd = 200; // 근접 처치 시 점수
		break;
	case WPTYPE::RIFLE:
		scoreToAdd = 100; // 총기 처치 시 점수
		break;

	case WPTYPE::FIST:
		
	default:
		scoreToAdd = 50; //기본 점수
		break;
	}

	_currentCombo++;
	_comboTimer = COMBO_TIME_LIMIT;

	float earnedScore = BASE_KILL_SCORE + ((_currentCombo - 1) * COMBO_BONUS_SCORE);
	_totalScore += earnedScore;
}

//처형 처치시(새로 추가함)
void SceneManager::OnEnemyExecuted()
{
	int scoreToAdd = 300; // 처형은 더 높은 점수 부여!

	_currentCombo++;
	_totalScore += (scoreToAdd * _currentCombo);
}


void SceneManager::ResetAllScore()
{
	_totalScore = 0.f;
	_savedScore = 0.f;
	_currentCombo = 0;
	_comboTimer = 0.f;
	_isGameEnded = false;
}

