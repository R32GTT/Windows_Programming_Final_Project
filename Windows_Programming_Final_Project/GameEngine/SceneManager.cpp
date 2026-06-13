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
		
		_playTime += dt;

		if (_currentCombo > 0)
		{

			// 현재 2.0f(2배속)로 설정해 두었으며, 원하는 속도에 맞춰 조절해 봅시다.
			float comboDecayMultiplier = 1.7f;
			_comboTimer -= (dt * comboDecayMultiplier);

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

void SceneManager::ResetCurrentMap() // 여기서 점수 리셋 해줘야 함
{
	if (_scene == nullptr) return;

	_totalScore = _savedScore;
	_currentCombo = 0;
	_comboTimer = 0.f;

	//추가: 맵을 재시작하면 누적 플레이 시간도 이전 세이브 포인트로 돌린다
	_playTime = _savedPlayTime;

	_scene->Clear();
	MapData loadedMapData = GET_SINGLE(DataManager)->GetCurrentMapData();

	_scene->BuildMapFromData(loadedMapData);
	_scene->Init();
}

//이게 CheckStageClear
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

	//추가: 맵을 무사히 클리어하고 넘어갈 때 현재까지의 시간을 저장합니다
	_savedPlayTime = _playTime;

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

			//추가: 최종 게임 클리어 시 시간 점수를 계산하여 더합니다.
			AddTimeScore();

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
		scoreToAdd = 300; // 근접 처치 시 점수
		break;
	case WPTYPE::RIFLE:
		scoreToAdd = 200; // 총기 처치 시 점수
		break;

	case WPTYPE::FIST:
		
	default:
		scoreToAdd = 30; //기본 점수
		break;
	}

	_currentCombo++;
	_comboTimer = COMBO_TIME_LIMIT;

	float earnedScore = BASE_KILL_SCORE + ((_currentCombo - 1) * COMBO_BONUS_SCORE);
	_totalScore += scoreToAdd;
}

//처형 처치시(새로 추가함)
void SceneManager::OnEnemyExecuted()
{
	int scoreToAdd = 400; // 처형은 더 높은 점수 부여!

	_currentCombo++;
	_totalScore += (scoreToAdd * _currentCombo);
}


void SceneManager::ResetAllScore()
{
	_totalScore = 0.f;
	_savedScore = 0.f;
	_currentCombo = 0;
	_comboTimer = 0.f;

	//시간 관련 변수도 모두 초기화 한다
	_playTime = 0.0f;
	_savedPlayTime = 0.0f;

	_isGameEnded = false;
}

// [추가] 시간 점수를 계산하는 함수 구현
void SceneManager::AddTimeScore()
{
	// 기획 예시: 기본 보너스 10000점에서 시작하여 1초당 50점씩 차감 (최소 0점 보장)
	int baseTimeScore = 10000;
	int timePenalty = static_cast<int>(_playTime * 50.0f);

	// 점수가 음수가 되지 않도록 std::max 사용 (algorithm 헤더 인클루드 필요)
	int finalTimeScore = std::max(0, baseTimeScore - timePenalty);

	_totalScore += finalTimeScore;
}

