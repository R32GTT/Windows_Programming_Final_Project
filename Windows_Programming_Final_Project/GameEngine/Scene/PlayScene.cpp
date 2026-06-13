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
	//GET_SINGLE(DataManager)->LoadMapData(L"TestMap1.json");

	//const MapData& mapData = GET_SINGLE(DataManager)->GetCurrentMapData();

	//Super::BuildMapFromData(mapData);

	Super::Init();

	if (Super::GetPlayer() != nullptr)
	{
		Super::SetCamOwner(Super::GetPlayer());
	}

	// === [추가] 스테이지 시작 시 적들의 경보(추격 상태)를 초기화해 줍니다. ===
	Enemy::s_isAlerted = false;

	//씬을 새로 불러와 시작할 때 무조건 콤보와 타이머를 0으로 초기화 합니다
	GET_SINGLE(SceneManager)->SetCurrentCombo(0);
	GET_SINGLE(SceneManager)->SetComboTimer(0.0f);

	//점수도 마찬가지로
	GET_SINGLE(SceneManager)->SetTotalScore(0);

}

void PlayScene::Update()
{
	// === [추가] 매 프레임 적들의 정적 변수에 플레이어 최신 위치를 동기화해 줍니다. ===
	GameObject* player = GetPlayer();
	if (player != nullptr)
	{
		Enemy::s_playerPos = player->GetPos();
	}

	Super::Update();

	//매 프레임마다 플레이어가 도착점에 닿았는지 검사
	//CheckStageClear();

	//플레이 도중 F5를 누르면 다시 EditScene으로 복귀
	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::F5))
	{
		GET_SINGLE(SceneManager)->ChangeScene(SceneType::EDITSCENE, L"TestMap1.json");
	}

}

void PlayScene::Render(ID2D1RenderTarget* renderTarget, float alpha)
{
	//Super::Render(renderTarget, alpha);
	// 1. 기존 게임 화면(맵, 플레이어, 적 등)을 먼저 그립니다.
	Super::Render(renderTarget, alpha);

	// 2. SceneManager에서 점수 및 상태 정보 가져오기
	int currentScore = GET_SINGLE(SceneManager)->GetTotalScore();
	int currentCombo = GET_SINGLE(SceneManager)->GetCurrentCombo();
	float comboTimer = GET_SINGLE(SceneManager)->GetComboTimer();
	bool isGameEnded = GET_SINGLE(SceneManager)->GetIsGameEnded();

	//추가: 현재 플레이 타임 가져오기
	float currentPlayTime = GET_SINGLE(SceneManager)->GetPlayTime();

	std::wstring scoreText;

	if (!isGameEnded)
	{
		// [인게임 상태] 점수와 콤보 표시
		scoreText = L"SCORE: " + std::to_wstring(currentScore);
		scoreText += L"\nTIME: " + std::to_wstring((int)currentPlayTime) + L"s";


		if (currentCombo > 0)
		{
			scoreText += L"\nCOMBO: " + std::to_wstring(currentCombo) + L" ( " + std::to_wstring((int)comboTimer) + L"s )";
		}
	}
	else
	{
		// [게임 종료 상태] 최종 점수 표시 + 시간을 표시한다
		scoreText = L"=== STAGE CLEAR ===\n\n";
		scoreText += L"CLEAR TIME: " + std::to_wstring((int)currentPlayTime) + L"s\n";
		scoreText += L"FINAL SCORE: " + std::to_wstring(currentScore);
	}

	// 3.폰트 설정을 따로 안 만들었을 때 사용하는 자체 폰트 생성 로직 (static 활용)
	static IDWriteFactory* dwriteFactory = nullptr;
	static IDWriteTextFormat* textFormat = nullptr;

	// 게임 실행 후 처음 이 함수가 실행될 때 딱 한 번만 폰트를 생성합니다.
	if (dwriteFactory == nullptr)
	{
		// DirectWrite 팩토리 생성
		DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(IDWriteFactory),
			reinterpret_cast<IUnknown**>(&dwriteFactory)
		);

		if (dwriteFactory)
		{
			// 폰트 포맷 생성 (맑은 고딕, 크기 22.0f, 굵게 설정)
			dwriteFactory->CreateTextFormat(
				L"맑은 고딕",					// 폰트 이름
				nullptr,						// 폰트 컬렉션 (기본)
				DWRITE_FONT_WEIGHT_BOLD,		// 글자 두께 (굵게)
				DWRITE_FONT_STYLE_NORMAL,		// 스타일
				DWRITE_FONT_STRETCH_NORMAL,		// 늘림 상태
				22.0f,							// 폰트 크기 (원하는 크기로 변경 가능)
				L"ko-kr",						// 국가 언어 설정
				&textFormat
			);
		}
	}

	// 4. 생성된 폰트와 브러시를 사용하여 화면에 텍스트 출력
	if (textFormat)
	{
		ID2D1SolidColorBrush* textBrush = nullptr;
		// 글자 색상을 흰색(White)으로 지정하여 브러시 생성
		renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &textBrush);

		if (textBrush)
		{
			//renderTarget에서 현재 화면의 전체 크기를 가져옵니다.
			D2D1_SIZE_F renderSize = renderTarget->GetSize();

			D2D1_RECT_F rect;

			if (!isGameEnded)
			{
				// [인게임 상태] 우측 상단 배치
				// 사각형을 화면 전체 너비로 잡고, 오른쪽 끝에서 20픽셀 여백을 둡니다.
				rect = D2D1::RectF(0.0f, 20.0f, renderSize.width - 20.0f, 200.0f);

				//가로: 우측 정렬 / 세로: 상단 정렬
				textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
				textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
			}
			else
			{
				// [게임 종료 상태] 화면 정중앙 배치
				// 사각형을 화면 전체 크기와 동일하게 꽉 채웁니다.
				rect = D2D1::RectF(0.0f, 0.0f, renderSize.width, renderSize.height);

				//가로: 중앙 정렬 / 세로: 중앙 정렬
				textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
				textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
			}

			// 화면에 텍스트 그리기 명령어 실행!
			renderTarget->DrawTextW(
				scoreText.c_str(),
				scoreText.length(),
				textFormat,
				rect,
				textBrush
			);

			// 사용이 끝난 브러시 메모리 해제
			textBrush->Release();
		}
	}

}

//CheckStageClear 추가됨
//void PlayScene::CheckStageClear() // Scene으로 옮길지 고민중
//{
//	if (_isStageClearing) return;
//
//	GameObject* player = GetPlayer();
//	if (player == nullptr) return;
//
//	std::vector<GameObject*> allObjects = GetAllObjects();
//
//	for (GameObject* obj : allObjects)
//	{
//		// 1. 오브젝트가 도착점(EndPoint)인지 확인합니다.
//		if (obj->GetObjectType() == OBJECTTYPE::ENDPOINT) // (사용중인 enum으로 변경하세요)
//		{
//			// 2. 플레이어와 EndPoint 간의 거리를 계산하거나 충돌 박스를 겹쳐봅니다.
//			Vec2F playerPos = player->GetPos();
//			Vec2F endPos = obj->GetPos();
//
//			// 예시: 두 객체 간의 거리(유클리드 거리)가 50 이하일 때 도달한 것으로 판정
//			float dx = playerPos.x - endPos.x;
//			float dy = playerPos.y - endPos.y;
//			float distance = sqrt((dx * dx) + (dy * dy));
//
//			if (distance < 50.0f) // (50.0f는 플레이어와 도착점의 충돌 판정 크기에 맞게 조절)
//			{
//				_isStageClearing = true;
//				// 3. 도달했다면 클리어 트리거를 작동시킵니다.
//				OnStageClearTrigger();
//				return;
//			}
//		}
//	}
//
//}

//추가된 함수들
void PlayScene::OnStageClearTrigger()
{
	int nextMapIdx = GET_SINGLE(DataManager)->GetCurrentMapIdx() + 1;
	std::string nextMapInfo = std::to_string(nextMapIdx);

	//다음 맵으로 이동을 시도
	bool hasNextMap = GET_SINGLE(DataManager)->GoToNextMap(nextMapInfo);

	if (hasNextMap)
	{
		Super::Clear();
		const MapData& mapData = GET_SINGLE(DataManager)->GetCurrentMapData();
		Super::BuildMapFromData(mapData); // 다음 맵의 오브젝트 소환

		if (GetPlayer() != nullptr)
		{
			SetCamOwner(GetPlayer());
		}
		Super::Init();

		//다음 맵이 시작 시 콤보가 초기화되도록 설정
		//GET_SINGLE(SceneManager)->SetCurrentCombo(0);
		//GET_SINGLE(SceneManager)->SetComboTimer(0.0f);

		_isStageClearing = false;
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
		Super::Clear();
		const MapData& mapData = GET_SINGLE(DataManager)->GetCurrentMapData();
		Super::BuildMapFromData(mapData);

		//참이면 다음 챕터로
		if (GetPlayer() != nullptr)
		{
			SetCamOwner(GetPlayer());
		}
		Super::Init();
		_isStageClearing = false;

	}
	else
	{
		//최종 게임 클리어시
		GET_SINGLE(DataManager)->EndGame();

		//SceneManager에 게임이 종료되었음을 알림
		GET_SINGLE(SceneManager)->SetIsGameEnded(true);

	}


}