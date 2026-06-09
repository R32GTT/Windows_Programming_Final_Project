#include "pch.h"
#include "PlayScene.h"
#include "Managers.h"
#include "DataManager.h"
#include "../Objects/Objects.h"

PlayScene::PlayScene()
{
}

PlayScene::~PlayScene()
{
}

void PlayScene::Init()
{
	GET_SINGLE(DataManager)->LoadMapData(L"TestMap1.json");

	const MapData& mapData = GET_SINGLE(DataManager)->GetCurrentMapData();

	Super::BuildMapFromData(mapData);

	Super::Init();
}

void PlayScene::Update()
{
	Super::Update();

	//매 프레임마다 플레이어가 도착점에 닿았는지 검사
	CheckStageClear();

}

void PlayScene::Render(ID2D1RenderTarget* renderTarget, float alpha)
{
	Super::Render(renderTarget, alpha);
}

//CheckStageClear 추가됨
void PlayScene::CheckStageClear() // Scene으로 옮길지 고민중
{
	GameObject* player = GetPlayer();
	if (player == nullptr) return;

	std::vector<GameObject*> allObjects = GetAllObjects();

	for (GameObject* obj : allObjects)
	{
		
	}

}

//추가된 함수들
void PlayScene::OnStageClearTrigger()
{
	int nextMapIdx = GET_SINGLE(DataManager)->GetCurrentMapIdx() + 1;
	std::string nextMapInfo = std::to_string(nextMapIdx);

	//다음 맵으로 이동을 시도
	bool hasNextMap = GET_SINGLE(DataManager)->GoToNextMap(nextMapInfo);

	if (hasNextMap)
	{
		//다음 스테이지 이동 성공
		if (GetPlayer() != nullptr)
		{
			SetCamOwner(GetPlayer());
		}
		Super::Init();
	} 
	else
	{

		//현재 챕터의 모든 스테이지 클리어
		HandleChapterClearSequence();
	}

}

void PlayScene::HandleChapterClearSequence()
{
	//플레이어 이동을 멈추고 정산창을 띄우는 연출 공간

	
	ProceedToNextChapter();
}


void PlayScene::ProceedToNextChapter()
{
	//DataManager에게 다음 챕터 진행을 요청한다
	bool hasNextChapter = GET_SINGLE(DataManager)->GoToNextChapter();

	if (hasNextChapter)
	{
		//참이면 다음 챕터로
		if (GetPlayer() != nullptr)
		{
			SetCamOwner(GetPlayer());
		}
		Super::Init();

	}
	else
	{
		//최종 게임 클리어시
		GET_SINGLE(DataManager)->EndGame();
	}


}