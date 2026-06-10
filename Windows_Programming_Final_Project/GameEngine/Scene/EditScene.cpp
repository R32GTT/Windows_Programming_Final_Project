#include "pch.h"
#include "EditScene.h"
#include "Managers.h"
#include "Camera.h"
#include "../LevelData/LevelData.h"
#include <fstream>
#include "../Objects/Objects.h"


EditScene::EditScene()
{

}

EditScene::~EditScene()
{

}

void EditScene::Init()
{

	Super::Init();

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
		// [Wall 전용] 마우스를 누를 때 시작점 저장
		if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::LeftMouse))
		{
			_wallStart = { snapX, snapY };
			_isDraggingWall = true;
		}
		// 마우스를 뗄 때 끝점 저장 후 생성!
		else if (GET_SINGLE(InputManager)->GetButtonUp(KeyType::LeftMouse) && _isDraggingWall)
		{
			POINT wallEnd = { snapX, snapY };

			// 1칸짜리를 클릭만 해도 생성되도록 방어 코드 추가
			if (wallEnd.x == _wallStart.x && wallEnd.y == _wallStart.y) {
				wallEnd.x += _gridSize;
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
		if (GET_SINGLE(InputManager)->GetButton(KeyType::LeftMouse))
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

		for (GameObject* obj : GetAllObjects())
		{
			Vec2F objPos = obj->GetPos();
			Vec2F halfSize = obj->GetHalfSize();

			if (mouseWorldPos.x >= objPos.x - halfSize.x &&
				mouseWorldPos.x <= objPos.x + halfSize.x &&
				mouseWorldPos.y >= objPos.y - halfSize.y &&
				mouseWorldPos.y <= objPos.y + halfSize.y)
			{
				
				if (_selectedObject == obj)
				{
					if (_selectedObject->GetObjectType() == OBJECTTYPE::PLAYER)
					{
						Super::SetPlayer(nullptr); _playerSpawned = false;
					}
					_selectedObject = nullptr;
				}
				
				RemoveObject(obj);
				break;
			}
		}
	}

	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::RightMouse))
	{
		Vec2F mouseWorldPos{ worldX, worldY };
		_selectedObject = nullptr; // 일단 선택 해제

		for (GameObject* obj : GetAllObjects())
		{
			// 오브젝트 클릭 시 해당 오브젝트를 선택 처리
			if (mouseWorldPos.x >= obj->GetPos().x - obj->GetHalfSize().x &&
				mouseWorldPos.x <= obj->GetPos().x + obj->GetHalfSize().x &&
				mouseWorldPos.y >= obj->GetPos().y - obj->GetHalfSize().y &&
				mouseWorldPos.y <= obj->GetPos().y + obj->GetHalfSize().y)
			{
				_selectedObject = obj;
				break;
			}
		}
	}

	// 선택된 상태에서 스프라이트(속성) 변경 (예: Q키를 누르면)
	if (_selectedObject != nullptr && GET_SINGLE(InputManager)->GetButtonDown(KeyType::Q))
	{
		if (_selectedObject->GetObjectType() == OBJECTTYPE::DECO)
		{
			// TODO: Deco의 스프라이트 종류를 순환시키는 함수 호출
			// _selectedObject->ChangeNextSprite();
		}
	}

	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::Return))
	{
		SaveMap(L"TestMap1.json");
	}

	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::F1))
	{
		LoadMap(L"TestMap1.json");
	}

	if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::F5))
	{
		SaveMap(L"TestMap1.json");

		GET_SINGLE(SceneManager)->ChangeScene(SceneType::DEVSCENE, L"TestMap1.json");
	}
}

void EditScene::Render(ID2D1RenderTarget* renderTarget, float alpha)
{
	renderTarget->Clear(D2D1::ColorF(0.2f, 0.2f, 0.2f));

	Vec2<float> camPos = GET_SINGLE(SceneManager)->GetCameraPos();
	float halfWinX = WinSizeX / 2.0f;
	float halfWinY = WinSizeY / 2.0f;

	// --- 1. 그리드(Grid) 그리기 ---
	ID2D1SolidColorBrush* gridBrush = nullptr;
	renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Gray, 0.5f), &gridBrush);

	if (gridBrush)
	{
		// 카메라 좌표에서 화면 절반을 뺀 진짜 월드 시작점 계산
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

	// --- 2. 마우스 미리보기 사각형 그리기 ---
	POINT curmousePos = GET_SINGLE(InputManager)->GetMousePos();

	// Update()와 동일한 공식 적용!
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
}


