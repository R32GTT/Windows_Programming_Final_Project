#include "pch.h"
#include "EditScene.h"
#include "Managers.h"
#include "Camera.h"
#include "../LevelData/LevelData.h"
#include <fstream>
#include <commdlg.h>
#include <filesystem>
#include "../Objects/Objects.h"


// 1. [저장] 다이얼로그: 유저가 파일 이름을 지정해서 저장할 때 사용
std::wstring SaveMapFileDialog(HWND hWnd)
{
	wchar_t szFile[260] = { 0 };
	OPENFILENAMEW ofn;
	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile) / sizeof(wchar_t);
	ofn.lpstrFilter = L"JSON Files (*.json)\0*.json\0All Files (*.*)\0*.*\0";
	ofn.nFilterIndex = 1;

	// 기본 디렉토리를 에셋 폴더로 강제 지정
	std::wstring initialDir = GET_SINGLE(FileManager)->GetFilePath().wstring();
	ofn.lpstrInitialDir = initialDir.c_str();

	// OFN_OVERWRITEPROMPT: 이미 있는 파일이면 덮어쓸지 물어봄
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

	if (GetSaveFileNameW(&ofn) == TRUE)
	{
		return std::wstring(szFile); // 절대 경로 반환
	}
	return L"";
}

// 2. [불러오기] 다이얼로그: 이미 만들어진 맵을 에디터로 로드할 때 사용
std::wstring LoadMapFileDialog(HWND hWnd)
{
	wchar_t szFile[260] = { 0 };
	OPENFILENAMEW ofn;
	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile) / sizeof(wchar_t);
	ofn.lpstrFilter = L"JSON Files (*.json)\0*.json\0All Files (*.*)\0*.*\0";
	ofn.nFilterIndex = 1;

	// 기본 디렉토리를 에셋 폴더로 강제 지정
	std::wstring initialDir = GET_SINGLE(FileManager)->GetFilePath().wstring();
	ofn.lpstrInitialDir = initialDir.c_str();

	// 로드할 때는 당연히 파일이 실제로 존재해야 하므로 덮어쓰기 프롬프트 없이 필수 체크만 진행
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

	if (GetOpenFileNameW(&ofn) == TRUE)
	{
		return std::wstring(szFile); // 절대 경로 반환
	}
	return L"";
}

std::string SelectMapFileDialog(HWND hWnd)
{
	wchar_t szFile[260] = { 0 };
	OPENFILENAMEW ofn;
	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hWnd; // NULL로 넘겨도 정상 작동함
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile) / sizeof(wchar_t);
	ofn.lpstrFilter = L"JSON Files (*.json)\0*.json\0All Files (*.*)\0*.*\0";
	ofn.nFilterIndex = 1;

	// 기본 디렉토리를 에셋 폴더로 강제 지정
	std::wstring initialDir = GET_SINGLE(FileManager)->GetFilePath().wstring();
	ofn.lpstrInitialDir = initialDir.c_str();

	// 파일이 실제로 존재하는지 필수 체크
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

	if (GetOpenFileNameW(&ofn) == TRUE)
	{
		std::filesystem::path filePath(szFile);
		return filePath.stem().string();
	}
	return "";
}


EditScene::EditScene()
{

}

EditScene::~EditScene()
{

}

void EditScene::Init()
{

	Super::Init();
	if (GetPlayer() != nullptr)
		_playerSpawned = true;
}

void EditScene::Update()
{
	_cam.SavePrevPos();

	float speed = 3000.f * GET_SINGLE(TimeManager)->GetDeltaTime();
	Vec2F camPos = _cam.GetPos();

	if (GET_SINGLE(InputManager)->GetButton(KeyType::W)) camPos.y -= speed;
	if (GET_SINGLE(InputManager)->GetButton(KeyType::S)) camPos.y += speed;
	if (GET_SINGLE(InputManager)->GetButton(KeyType::A)) camPos.x -= speed;
	if (GET_SINGLE(InputManager)->GetButton(KeyType::D)) camPos.x += speed;

	_cam.SetPos(camPos);
	_cam.TickComp();
	GET_SINGLE(SceneManager)->SetCameraPos(camPos);

	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::KEY_1)) _currentEntity = EntityType::PlayerSpawn;
	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::KEY_2)) _currentEntity = EntityType::Enemy;
	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::KEY_3)) _currentEntity = EntityType::Weapon;
	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::KEY_4)) _currentEntity = EntityType::Wall;
	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::KEY_5)) _currentEntity = EntityType::Deco;
	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::KEY_6)) _currentEntity = EntityType::Endpoint;

	POINT curmousePos = GET_SINGLE(InputManager)->GetMousePos();

	float halfWinX = WinSizeX / 2.0f;
	float halfWinY = WinSizeY / 2.0f;

	float worldX = curmousePos.x + _cam.GetPos().x - halfWinX;
	float worldY = curmousePos.y + _cam.GetPos().y - halfWinY;

	int snapX = (int)(floor(worldX / _gridSize)) * _gridSize;
	int snapY = (int)(floor(worldY / _gridSize)) * _gridSize;

	if (_currentEntity == EntityType::Wall)
	{

		if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::LeftMouse))
		{
			_wallStart = { snapX, snapY };
			_isDraggingWall = true;
		}
		else if (GET_SINGLE(InputManager)->GetButtonUp(KeyType::LeftMouse) && _isDraggingWall)
		{
			POINT wallEnd = { snapX, snapY };

			if (wallEnd.x == _wallStart.x)
			{
				wallEnd.x += _gridSize;
			}
			if (wallEnd.y == _wallStart.y)
			{
				wallEnd.y += _gridSize;
			}

			Wall* newWall = new Wall(_wallStart, wallEnd);
			newWall->Init(); // 필요시 초기화 호출
			AddObject(newWall);
			_isDraggingWall = false;
		}
	}
	else // Wall이 아닌 일반 엔티티들 (Point 스폰 + 홀드 드래그 가능)
	{
		if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::LeftMouse))
		{
			if (snapX != _lastPos.x || snapY != _lastPos.y)
			{
				GameObject* newObj = nullptr;

				switch (_currentEntity)
				{
				case EntityType::NONE:
					break;
				case EntityType::PlayerSpawn:
					if (!_playerSpawned) // 플레이어는 무조건 하나여야 하니까
					{
						newObj = new Player();
						Super::SetPlayer(newObj);
						_playerSpawned = true;
					}
					break;
				case EntityType::Deco:
					newObj = new DECO();
					break;
				case EntityType::Enemy:
					newObj = new Enemy();
					break;
				case EntityType::Weapon:
					newObj = new Weapon();
					break;
				case EntityType::Endpoint:
					newObj = new EndPoint();
					break;
				default:
					break;
				}


				if (newObj != nullptr)
				{
					newObj->Init();
					newObj->SetPos(Vec2F{ (float)snapX, (float)snapY }); // 스폰 위치 세팅
					newObj->SavePrevPos();
					AddObject(newObj);
				}

				_lastPos = { snapX, snapY };
			}
		}
		else if (GET_SINGLE(InputManager)->GetButtonUp(KeyType::LeftMouse))
		{
			_lastPos = { -9999, -9999 };
		}
	}

	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::Delete))
	{
		Vec2F mouseWorldPos{ worldX, worldY };
		float padding = 8.0f;

		for (GameObject* obj : GetAllObjects())
		{
			Vec2F objPos = obj->GetPos();
			Vec2F halfSize = obj->GetHalfSize();

			if (mouseWorldPos.x >= objPos.x - halfSize.x - padding &&
				mouseWorldPos.x <= objPos.x + halfSize.x + padding &&
				mouseWorldPos.y >= objPos.y - halfSize.y - padding &&
				mouseWorldPos.y <= objPos.y + halfSize.y + padding)
			{

				if (_selectedObject == obj)
				{
					if (_selectedObject->GetObjectType() == OBJECTTYPE::PLAYER)
					{
						Super::SetPlayer(nullptr);
						_playerSpawned = false;
					}
					_selectedObject = nullptr;
					RemoveObject(obj);
					break;
				}
			}
		}
	}

	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::RightMouse))
	{
		Vec2F mouseWorldPos{ worldX, worldY };
		float padding = 8.0f;
		_selectedObject = nullptr; // 일단 선택 해제

		for (GameObject* obj : GetAllObjects())
		{
			if (mouseWorldPos.x >= obj->GetPos().x - obj->GetHalfSize().x - padding &&
				mouseWorldPos.x <= obj->GetPos().x + obj->GetHalfSize().x + padding &&
				mouseWorldPos.y >= obj->GetPos().y - obj->GetHalfSize().y - padding &&
				mouseWorldPos.y <= obj->GetPos().y + obj->GetHalfSize().y + padding)
			{
				_selectedObject = obj;
				break;
			}
		}

		//// 선택된 상태에서 스프라이트(속성) 변경 (예: Q키를 누르면)
		//if (_selectedObject != nullptr && GET_SINGLE(InputManager)->GetButtonDown(KeyType::Q))
		//{
		//	if (_selectedObject->GetObjectType() == OBJECTTYPE::DECO)
		//	{
		//		// TODO: Deco의 스프라이트 종류를 순환시키는 함수 호출
		//		// _selectedObject->ChangeNextSprite();
		//	}
		//}
	}


	if (_selectedObject != nullptr)
	{
		OBJECTTYPE objType = _selectedObject->GetObjectType();

		int adjustDir = 0;
		if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::Left))  adjustDir = -1;
		if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::Right)) adjustDir = 1;

		
		if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::Up) ||
			GET_SINGLE(InputManager)->GetButtonDown(KeyType::Down))
		{
			_indexCursor = (_indexCursor + 1) % 3;
		}
		if (objType == OBJECTTYPE::ENEMY)
		{
			Enemy* enemy = static_cast<Enemy*>(_selectedObject);

			// [기존 방향키 순환 시스템 활용]
			if (adjustDir != 0)
			{
				if (_indexCursor == 0) // 커서 0: 적 타입(Normal, Armored)
				{
					int currentEType = static_cast<int>(enemy->GetEType());
					int maxCount = static_cast<int>(EnemyType::ETYPE_COUNT);
					currentEType = (currentEType + adjustDir + maxCount) % maxCount;
					enemy->SetEnemyType(static_cast<EnemyType>(currentEType));
				}
				else if (_indexCursor == 1) // 커서 1: 무기 타입(Fist, Crowbar, Rifle, Random)
				{
					int currentWType = static_cast<int>(enemy->GetWPTYPE());
					int maxCount = static_cast<int>(WPTYPE::TOTAL_COUNT);
					currentWType = (currentWType + adjustDir + maxCount) % maxCount;
					enemy->SetWPTYPE(static_cast<WPTYPE>(currentWType));
				}
				// ==========================================
				// [여기에 추가!] 커서 2: 기본 행동 상태(IDLE, PATROL, ROAMING)
				// ==========================================
				else if (_indexCursor == 2)
				{
					int currentState = static_cast<int>(enemy->GetDefaultState());

					// 기본 상태로 쓸 수 있는 건 3개뿐 (IDLE=0, PATROL=1, ROAMING=2)
					// (CHASE나 ATTACK으로 스폰시킬 수는 없으므로 3으로 고정합니다)
					int maxStateCount = 3;

					currentState = (currentState + adjustDir + maxStateCount) % maxStateCount;
					enemy->SetDefaultState(static_cast<EnemyState>(currentState));
				}
			}

			// (아래는 기존에 작성하신 무기 다이렉트 단축키 그대로 유지)
			if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::KEY_0))
			{
				enemy->SetWPTYPE(WPTYPE::RANDOM_ANY);
			}
			else if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::KEY_7))
			{
				enemy->SetWPTYPE(WPTYPE::FIST);
			}
			else if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::KEY_8))
			{
				enemy->SetWPTYPE(WPTYPE::CROWBAR);
			}
			else if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::KEY_9))
			{
				enemy->SetWPTYPE(WPTYPE::RIFLE);
			}
		}
		else if (objType == OBJECTTYPE::WEAPON)
		{
			Weapon* weapon = static_cast<Weapon*>(_selectedObject);

			if (adjustDir != 0)
			{
				int currentWType = static_cast<int>(weapon->GetWeaponType());
				int maxCount = static_cast<int>(WPTYPE::TOTAL_COUNT);
				currentWType = (currentWType + adjustDir + maxCount) % maxCount;
				weapon->SetWeaponType(static_cast<WPTYPE>(currentWType));
			}

			if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::KEY_0))
			{
				weapon->SetWeaponType(WPTYPE::RANDOM_ANY);
			}
			else if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::KEY_8))
			{
				weapon->SetWeaponType(WPTYPE::CROWBAR);
			}
			else if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::KEY_9))
			{
				weapon->SetWeaponType(WPTYPE::RIFLE);
			}
		}
		if (_selectedObject->GetObjectType() != OBJECTTYPE::ENDPOINT &&
			_selectedObject->GetObjectType() != OBJECTTYPE::WALL)
		{
			if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::R))
			{
				float currentAngle = _selectedObject->GetRotationAngle();
				currentAngle += 45.0f;

				if (currentAngle >= 360.0f)
					currentAngle -= 360.0f;

				_selectedObject->SetRotationAngle(currentAngle);
			}
		}
	}

	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::F8))
	{
		std::wstring fullPath = SaveMapFileDialog(nullptr);

		if (!fullPath.empty())
		{
			std::wstring fileName = std::filesystem::path(fullPath).filename().wstring();
			SaveMap(fileName);
		}
	}

	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::F1))
	{
		std::wstring fullPath = LoadMapFileDialog(nullptr);

		if (!fullPath.empty())
		{
			std::wstring fileName = std::filesystem::path(fullPath).filename().wstring();
			LoadMap(fileName);
		}
	}

	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::F5))
	{
		/*SaveMap(L"TestMap1.json");
		GET_SINGLE(SceneManager)->ChangeScene(SceneType::DEVSCENE, L"TestMap1.json");*/

		std::wstring fullPath = LoadMapFileDialog(nullptr);

		if (!fullPath.empty())
		{
			// 이름만 추출해서 LoadMap 호출
			std::wstring fileName = std::filesystem::path(fullPath).filename().wstring();
			GET_SINGLE(SceneManager)->ChangeScene(SceneType::DEVSCENE, fileName);
			return;
		}
	}

	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::F7))
	{
		Clear();
		_selectedObject = nullptr;
		_playerSpawned = false;
	}


	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::F2))
	{
		GET_SINGLE(SceneManager)->SetCurrentCombo(0);
		GET_SINGLE(SceneManager)->SetComboTimer(0.0f);

		GET_SINGLE(SceneManager)->SetTotalScore(0);

		GET_SINGLE(SceneManager)->SetIsGameEnded(false);

		std::wstring fullPath = LoadMapFileDialog(nullptr);

		if (!fullPath.empty())
		{
			std::wstring fileName = std::filesystem::path(fullPath).filename().wstring();
			GET_SINGLE(SceneManager)->ChangeScene(SceneType::PLAYSCENE, fileName);
			return;
		}
	}

	Super::FlushSpawnQueue(); // 넣어둠. 다른거 해도 돼.
}

void EditScene::Render(ID2D1RenderTarget* renderTarget, float alpha)
{
	renderTarget->Clear(D2D1::ColorF(0.2f, 0.2f, 0.2f));

	Vec2<float> camPos = GET_SINGLE(SceneManager)->GetCameraPos();
	float halfWinX = WinSizeX / 2.0f;
	float halfWinY = WinSizeY / 2.0f;

	ID2D1SolidColorBrush* gridBrush = nullptr;
	renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Gray, 0.5f), &gridBrush);

	if (gridBrush)
	{
		int startX = (int)(floor((camPos.x - halfWinX) / _gridSize)) * _gridSize;
		int startY = (int)(floor((camPos.y - halfWinY) / _gridSize)) * _gridSize;

		for (int x = startX; x < startX + WinSizeX + _gridSize * 2; x += _gridSize) {
			Vec2F screenPos = GET_SINGLE(SceneManager)->ToRenderPos(Vec2F((float)x, 0));
			renderTarget->DrawLine(D2D1::Point2F(screenPos.x, 0), D2D1::Point2F(screenPos.x, WinSizeY), gridBrush);
		}
		for (int y = startY; y < startY + WinSizeY + _gridSize * 2; y += _gridSize) {
			Vec2F screenPos = GET_SINGLE(SceneManager)->ToRenderPos(Vec2F(0, (float)y));
			renderTarget->DrawLine(D2D1::Point2F(0, screenPos.y), D2D1::Point2F(WinSizeX, screenPos.y), gridBrush);
		}
		gridBrush->Release();
	}

	POINT curmousePos = GET_SINGLE(InputManager)->GetMousePos();

	float worldX = curmousePos.x + camPos.x - halfWinX;
	float worldY = curmousePos.y + camPos.y - halfWinY;

	int snapX = (int)(floor(worldX / _gridSize)) * _gridSize;
	int snapY = (int)(floor(worldY / _gridSize)) * _gridSize;

	Vec2F screenPos = GET_SINGLE(SceneManager)->ToRenderPos(Vec2F((float)snapX, (float)snapY));

	D2D1_RECT_F rect = D2D1::RectF(screenPos.x, screenPos.y, screenPos.x + _gridSize, screenPos.y + _gridSize);

	ID2D1SolidColorBrush* previewBrush = nullptr;
	renderTarget->CreateSolidColorBrush(D2D1::ColorF(1.0f, 1.0f, 0.0f, 0.5f), &previewBrush);
	if (previewBrush)
	{
		renderTarget->FillRectangle(rect, previewBrush);
		previewBrush->Release();
	}

	if (_selectedObject != nullptr)
	{
		Vec2F objPos = _selectedObject->GetPos();
		Vec2F halfSize = _selectedObject->GetHalfSize();

		Vec2F screenCenter = GET_SINGLE(SceneManager)->ToRenderPos(objPos);

		D2D1_RECT_F outlineRect = D2D1::RectF(
			screenCenter.x - halfSize.x,
			screenCenter.y - halfSize.y,
			screenCenter.x + halfSize.x,
			screenCenter.y + halfSize.y
		);


		D2D1::ColorF outlineColor = D2D1::ColorF(0.0f, 0.8f, 0.8f, 1.0f); // 기본 시안색 (0: 적 타입 수정)
		if (_indexCursor == 1) outlineColor = D2D1::ColorF::White;        // 흰색 (1: 무기 수정)
		else if (_indexCursor == 2) outlineColor = D2D1::ColorF::LimeGreen; // 밝은 초록색 (2: 상태 수정)

		ID2D1SolidColorBrush* outlineBrush = nullptr;
		renderTarget->CreateSolidColorBrush(outlineColor, &outlineBrush);

		if (outlineBrush)
		{
			renderTarget->DrawRectangle(outlineRect, outlineBrush, 2.0f);
			outlineBrush->Release();
		}

		// 무기 및 상태 인디케이터 그리기
		if (_selectedObject->GetObjectType() == OBJECTTYPE::ENEMY ||
			_selectedObject->GetObjectType() == OBJECTTYPE::WEAPON)
		{
			WPTYPE weaponType;

			if (_selectedObject->GetObjectType() == OBJECTTYPE::ENEMY)
				weaponType = static_cast<Enemy*>(_selectedObject)->GetWPTYPE();
			else
				weaponType = static_cast<Weapon*>(_selectedObject)->GetWeaponType();

			D2D1::ColorF indicatorColor = D2D1::ColorF::White;

			if (weaponType == WPTYPE::RANDOM_ANY)
				indicatorColor = D2D1::ColorF::Magenta;    // 랜덤 = 보라색
			else if (weaponType == WPTYPE::FIST)
				indicatorColor = D2D1::ColorF::LightGray;  // 주먹 = 밝은 회색
			else if (weaponType == WPTYPE::CROWBAR)
				indicatorColor = D2D1::ColorF::Red;        // 빠루 = 빨간색
			else if (weaponType == WPTYPE::RIFLE)
				indicatorColor = D2D1::ColorF::Yellow;     // 라이플 = 노란색

			// 1. 무기 인디케이터 그리기 (아래쪽 박스)
			D2D1_RECT_F indicatorRect = D2D1::RectF(
				screenCenter.x - 10.0f,
				screenCenter.y - halfSize.y - 15.0f,
				screenCenter.x + 10.0f,
				screenCenter.y - halfSize.y - 5.0f
			);

			ID2D1SolidColorBrush* indicatorBrush = nullptr;
			renderTarget->CreateSolidColorBrush(indicatorColor, &indicatorBrush);
			if (indicatorBrush)
			{
				renderTarget->FillRectangle(indicatorRect, indicatorBrush);
				indicatorBrush->Release();
			}

			if (_selectedObject->GetObjectType() == OBJECTTYPE::ENEMY)
			{
				EnemyState state = static_cast<Enemy*>(_selectedObject)->GetDefaultState();
				D2D1::ColorF stateColor = D2D1::ColorF::Black;

				if (state == EnemyState::IDLE)
					stateColor = D2D1::ColorF::CornflowerBlue; // IDLE = 파란색
				else if (state == EnemyState::PATROL)
					stateColor = D2D1::ColorF::LimeGreen;      // PATROL = 초록색
				else if (state == EnemyState::ROAMING)
					stateColor = D2D1::ColorF::Orange;         // ROAMING = 주황색

				// 무기 인디케이터보다 15픽셀 더 위쪽에 그립니다 (2층 탑 쌓기)
				D2D1_RECT_F stateRect = D2D1::RectF(
					screenCenter.x - 10.0f,
					screenCenter.y - halfSize.y - 30.0f,
					screenCenter.x + 10.0f,
					screenCenter.y - halfSize.y - 20.0f
				);

				ID2D1SolidColorBrush* stateBrush = nullptr;
				renderTarget->CreateSolidColorBrush(stateColor, &stateBrush);
				if (stateBrush)
				{
					renderTarget->FillRectangle(stateRect, stateBrush);
					stateBrush->Release();
				}
			}
		}
	}

	Super::Render(renderTarget, alpha); // 객체들 렌더링
}

void EditScene::SaveMap(const std::wstring& fileName)
{
	std::vector<ObjectSpawnData> mapDataList;

	for (GameObject* obj : GetAllObjects())
	{
		ObjectSpawnData data{};
		obj->SaveToData(data);
		mapDataList.push_back(data);
	}

	GET_SINGLE(DataManager)->SaveMapData(fileName, mapDataList);
}

void EditScene::LoadMap(const std::wstring& fileName)
{
	Super::Clear();

	GET_SINGLE(DataManager)->LoadMapData(fileName);
	BuildMapFromData(GET_SINGLE(DataManager)->GetCurrentMapData());
	Super::Init();

	int objCount = GetAllObjects().size();
	std::string msg = "Current Object Count: " + std::to_string(objCount) + "\n";
	OutputDebugStringA(msg.c_str());
}


