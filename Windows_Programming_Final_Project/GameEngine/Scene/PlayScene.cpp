#include "pch.h"
#include "PlayScene.h"
#include "Managers.h"
#include "DataManager.h"

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
}

void PlayScene::Render(ID2D1RenderTarget* renderTarget, float alpha)
{
	Super::Render(renderTarget, alpha);
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

	//
	ProceedToNextChapter();
}


void PlayScene::ProceedToNextChapter()
{
	//DataManager에게 다음 챕터 진행을 요청한다
	bool hasNextChapter = GET_SINGLE(DataManager)->GoToNextChapter();

	if (hasNextChapter)
	{
		//참이면 다음 챕터로
	}
	else
	{
		//최종 게임 클리어시
		GET_SINGLE(DataManager)->EndGame();
	}


}