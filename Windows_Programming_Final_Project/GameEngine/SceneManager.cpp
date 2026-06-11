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
			_totalScore = SCORE_TIME_PENALTY * dt;
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

	//화면 고정 레이어
	if (_sceneType == SceneType::PLAYSCENE)
	{

		// 렌더링에 사용할 텍스트 버퍼 생성
		std::wstring scoreText;

		//일단 글자를 그릴 하얀색 브러시를 임시로 생성합니다
		ID2D1SolidColorBrush* textBrush = nullptr;
		renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &textBrush);


		// (주의) 아래 렌더링 코드는 프로젝트 내의 폰트 매니저나 DrawText 함수로 교체해서 사용하세요!
		// 예시에서는 일반적인 엔진에서 많이 쓰는 DWrite 브러시와 포맷을 사용한다고 가정했습니다.

		if (!_isGameEnded)
		{
			// [인게임 상태] : 좌측 또는 우측 상단에 작게 점수 표시
			scoreText = L"SCORE: " + std::to_wstring(GetTotalScore());
			if (_currentCombo > 0)
			{
				scoreText += L"\nCOMBO: " + std::to_wstring(_currentCombo) + L" ( " + std::to_wstring((int)_comboTimer) + L"s )";
			}

			// 화면 좌측 상단 (예: x: 20, y: 20) 위치에 렌더링
			D2D1_RECT_F rect = D2D1::RectF(20, 20, 300, 100);
			// renderTarget->DrawTextW(scoreText.c_str(), scoreText.length(), TextFormat, rect, Brush);
		}
		else
		{
			// [게임 종료 상태] : 화면 중앙에 거대하게 결과 표시
			scoreText = L"=== GAME CLEAR ===\n\nFINAL SCORE: " + std::to_wstring(GetTotalScore());

			// 화면 전체 영역을 잡아두고 가운데 정렬(Center Alignment)로 렌더링하면 좋습니다.
			D2D1_RECT_F rect = D2D1::RectF(0, 0, winSizeV.x, winSizeV.y);
			// renderTarget->DrawTextW(scoreText.c_str(), scoreText.length(), LargeTextFormat, rect, Brush);
		}

		// 다 쓴 브러시는 메모리 누수를 막기 위해 해제해 줍니다.
		if (textBrush)
		{
			textBrush->Release();
		}

	}

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
		ChangeMap(GET_SINGLE(DataManager)->GetCurrentMapData());
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
void SceneManager::OnEnemyKilled()
{
	if (_isGameEnded) return;

	_currentCombo++;
	_comboTimer = COMBO_TIME_LIMIT;

	float earnedScore = BASE_KILL_SCORE + ((_currentCombo - 1) * COMBO_BONUS_SCORE);
	_totalScore += earnedScore;
}

void SceneManager::ResetAllScore()
{
	_totalScore = 0.f;
	_savedScore = 0.f;
	_currentCombo = 0;
	_comboTimer = 0.f;
	_isGameEnded = false;
}

