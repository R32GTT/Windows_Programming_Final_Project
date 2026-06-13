#include "pch.h"
#include "Scene.h"
#include "GameObject.h"
#include "../Objects/Objects.h"
#include "Managers.h"
#include "../Utility/Vec2.h"
#include "../LevelData/LevelData.h"	

static IDWriteFactory* s_dwriteFactory = nullptr;
static IDWriteTextFormat* s_textFormat = nullptr;
static ID2D1SolidColorBrush* s_uiBrush = nullptr;

Scene::Scene()
{
	
}

Scene::~Scene()
{
	for (int i = 0; i < (int)Layers::LAYER_COUNT; ++i)
	{
		for (GameObject* object : _objects[i])
		{
			SAFE_DELETE(object);
		}
		_objects[i].clear(); // 각 레이어별로 vector를 확실히 비워줌
	}
	_player = nullptr;
	_objectMap.clear();
	_spawnQueue.clear();
	if (s_uiBrush) { s_uiBrush->Release(); s_uiBrush = nullptr; }
	if (s_textFormat) { s_textFormat->Release(); s_textFormat = nullptr; }
	if (s_dwriteFactory) { s_dwriteFactory->Release(); s_dwriteFactory = nullptr; }

}

void Scene::Init()
{
	for (const std::vector<GameObject*>& objects : _objects)
		for (GameObject* object : objects)
			object->Init();

}

void Scene::Update()
{
	float deltaTime = GET_SINGLE(TimeManager)->GetDeltaTime();
	
	_cam.SavePrevPos();

	for (const std::vector<GameObject*>& objects : _objects)
		for (GameObject* object : objects)
			object->Update();

	GET_SINGLE(CollisionManager)->Update();

	PurgeDeadObjects();
	FlushSpawnQueue();
	_cam.TickComp();
}

void Scene::Render(ID2D1RenderTarget* renderTarget, float alpha)
{
	Vec2F camRenderPos = _cam.GetRenderPos(alpha);

	GET_SINGLE(SceneManager)->SetCameraPos(camRenderPos);

	for (const std::vector<GameObject*>& objects : _objects)
		for (GameObject* object : objects)
			object->Render(renderTarget, alpha);
	if (_player)
		_player->Render(renderTarget, alpha);

	if (_player)
	{

		if (s_dwriteFactory == nullptr)
		{
			DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&s_dwriteFactory));
			if (s_dwriteFactory)
			{
				s_dwriteFactory->CreateTextFormat(
					L"Consolas", NULL, DWRITE_FONT_WEIGHT_BOLD, DWRITE_FONT_STYLE_NORMAL,
					DWRITE_FONT_STRETCH_NORMAL, 24.0f, L"ko-kr", &s_textFormat
				);
			}
		}
		if (s_uiBrush == nullptr)
		{
			renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Yellow), &s_uiBrush);
		}

		if (s_textFormat && s_uiBrush)
		{
			D2D1_SIZE_F rtSize = renderTarget->GetSize();

			if (_player->IsKilled())
			{
				std::wstring restartText = L"PRESS 'R' TO RESTART";

				D2D1_RECT_F deadRect = D2D1::RectF(
					rtSize.width / 2.0f - 140.0f, 
					rtSize.height / 2.0f - 20.0f,
					rtSize.width,
					rtSize.height
				);

				renderTarget->DrawTextW(
					restartText.c_str(),
					(UINT32)restartText.length(),
					s_textFormat,
					deadRect,
					s_uiBrush
				);
			}
			else
			{
				D2D1_RECT_F textRect = D2D1::RectF(
					20.0f,
					rtSize.height - 40.0f,
					300.0f,
					rtSize.height
				);

				if (static_cast<Player*>(_player)->GetWeaponType() == WPTYPE::RIFLE)
				{
					int maxAmmo = static_cast<Player*>(_player)->GetMaxAmmo();
					int curAmmo = static_cast<Player*>(_player)->GetAmmo();
					std::wstring ammoText = L"";
					ammoText += std::format(L"Ammo {}/{}", curAmmo, maxAmmo);

					renderTarget->DrawTextW(
						ammoText.c_str(),
						(UINT32)ammoText.length(),
						s_textFormat,
						textRect,
						s_uiBrush
					);
				}
			}
		}
	}
}

void Scene::Clear()
{
	for (int i = 0; i < (int)Layers::LAYER_COUNT; ++i)
	{
		for (GameObject* object : _objects[i])
		{
			SAFE_DELETE(object);
		}
		_objects[i].clear(); // 각 레이어별로 vector를 확실히 비워줌
	}
	_player = nullptr;
	_objectMap.clear();
	_spawnQueue.clear();
}

void Scene::PurgeDeadObjects()
{
	for (int i = 0; i < (int)Layers::LAYER_COUNT; ++i)
	{
		auto& layerVec = _objects[i];

		// 조건(CheckDead)에 맞는 오브젝트들을 뒤로 밀고 지워버리는 가비지 컬렉션 메커니즘
		auto it = std::remove_if(layerVec.begin(), layerVec.end(), [this](GameObject* obj) {
			if (obj && obj->CheckDead())
			{
				_objectMap.erase(obj->GetID()); 
				SAFE_DELETE(obj);               
				return true;                    
			}
			return false;
			});

		layerVec.erase(it, layerVec.end());
	}
}

GameObject* Scene::GetGameObjectByID(unsigned int ID)
{
	auto iter = _objectMap.find(ID);

	if (iter != _objectMap.end())
		return iter->second;

	return nullptr;
}

bool Scene::CheckSceneCleared()
{
	std::vector<GameObject*> enemies = GetObjectsByLayer(Layers::ACTORS);

	for (GameObject* obj : enemies)
	{
		if (obj->GetObjectType() == OBJECTTYPE::ENEMY)
		{
			if (!obj->IsKilled())
				return false;
		}
	}

	return true;
}

void Scene::AddObject(GameObject* object)
{
	if (object == nullptr)
		return;
	_spawnQueue.push_back(object);
}

void Scene::FlushSpawnQueue()
{
	if (_spawnQueue.empty())
		return;

	for (GameObject* newObj : _spawnQueue)
	{
		_objects[(int)newObj->GetLayer()].push_back(newObj);
		_objectMap[newObj->GetID()] = newObj;
	}

	_spawnQueue.clear();
}

void Scene::RemoveObject(GameObject* object)
{
	if (object == nullptr)
		return;

	std::vector<GameObject*>& v = _objects[(int)object->GetLayer()];
	v.erase(std::remove(v.begin(), v.end(), object), v.end());
	_objectMap.erase(object->GetID());

	SAFE_DELETE(object);
}

void Scene::BuildMapFromData(const MapData& mapData)
{
	std::unordered_map<unsigned int, unsigned int> idMap;

	for (const ObjectSpawnData& spawnData : mapData.objects)
	{
		GameObject* newObj = CreateObjectFromData(spawnData);

		if (newObj != nullptr)
		{
			newObj->LoadFromData(spawnData);
			AddObject(newObj);
			if (newObj->GetObjectType() == OBJECTTYPE::PLAYER)
				SetPlayer(newObj);
			idMap[spawnData.fileID] = newObj->GetID();
		}
	}
	FlushSpawnQueue();
}

void Scene::LinkObjectReferences(const MapData& mapData, const std::unordered_map<unsigned int, unsigned int>& idMap)
{
	/* for (const ObjectSpawnData& spawnData : mapData.objects)
	{
		if (spawnData.targetId != 0)
		{
			unsigned int realOwnerId = idMap.at(spawnData.id);
			unsigned int realTargetId = idMap.at(spawnData.targetId);

			GameObject* owner = GetGameObjectByID(realOwnerId);
			GameObject* target = GetGameObjectByID(realTargetId);
			// owner->SetTarget(target);
		}
	}
	*/
}

//무기 스폰 함수
void Scene::SpawnWeapon(Vec2<float> pos) {

	Weapon* weapon = new Weapon();

	weapon->SetPos(pos);
	weapon->Init();

	AddObject(weapon);
}


GameObject* Scene::CreateObjectFromData(const ObjectSpawnData& data)
{
	GameObject* newObj = nullptr;
	switch (data.objType)
	{
	case OBJECTTYPE::NONE:
		break;
	case OBJECTTYPE::PLAYER:
		newObj = new Player();
		_player = newObj;
		break;
	case OBJECTTYPE::ENEMY:
		newObj = new Enemy();
		break;
	case OBJECTTYPE::WEAPON:
		newObj = new Weapon();
		break;
	case OBJECTTYPE::PROJECTILE:
		newObj = new Projectile();
		break;
	case OBJECTTYPE::DECO:
		newObj = new DECO();
		break;
	case OBJECTTYPE::WALL:
		newObj = new Wall();
		break;
	case OBJECTTYPE::FLOOR:
		newObj = new Floor();
		break;
	case OBJECTTYPE::ENDPOINT:
		newObj = new EndPoint();
		break;
	case OBJECTTYPE::OBJECT_COUNT:
		break;
	default:
		break;
	}
	return newObj;
}
