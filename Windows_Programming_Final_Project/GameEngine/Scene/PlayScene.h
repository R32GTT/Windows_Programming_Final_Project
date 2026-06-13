#pragma once
#include "Scene.h"

class PlayScene : public Scene
{
public:
	using Super = Scene;
	PlayScene();
	virtual ~PlayScene();

	virtual void Init() override;
	virtual void Update() override;
	virtual void Render(ID2D1RenderTarget* renderTarget, float alpha) override;

	//게임 흐림 제어를 위해 새로 추가된 함수들
	void CheckStageClear();               //플레이어가 도착점에 닿았는지 프레임마다 체크
	void OnStageClearTrigger();          //스테이지 클리어(포탈 충돌 등) 시 호출
	void HandleChapterClearSequence();  //챕터 클리어 연출 및 UI 처리
	void ProceedToNextChapter();       //실제로 다음 챕터로 전환

	bool _isStageClearing = false;


};