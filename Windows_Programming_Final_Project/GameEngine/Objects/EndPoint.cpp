#include "pch.h"
#include "EndPoint.h"
#include "../LevelData/LevelData.h"
#include "Managers.h"
#include "../Scene/Scene.h"

EndPoint::EndPoint()
{
	type = OBJECTTYPE::ENDPOINT;
	layer = Layers::ACTORS;
}

EndPoint::~EndPoint()
{
}

void EndPoint::Render(ID2D1RenderTarget* renderTarget, float alpha)
{
	if (!renderTarget) return;

	// 1. 렌더링 좌표 계산
	Vec2F screenPos = GetRenderPos(alpha);
	Vec2F ToRenderPos = GET_SINGLE(SceneManager)->ToRenderPos(screenPos);

	// 2. 화살표 색상 브러시 생성 (어두운 배경에 잘 어울리는 묵직한 청록색 톤)
	ID2D1SolidColorBrush* arrowBrush = nullptr;
	renderTarget->CreateSolidColorBrush(D2D1::ColorF(0.0f, 0.6f, 0.6f, 1.0f), &arrowBrush);

	if (arrowBrush)
	{
		float x = ToRenderPos.x;
		float y = ToRenderPos.y;

		float size = GetHalfSize().x;

		D2D1_POINT_2F top = D2D1::Point2F(x, y - size);                 // 화살표 끝점 (위)
		D2D1_POINT_2F bottom = D2D1::Point2F(x, y + size);              // 화살표 기둥 끝점 (아래)
		D2D1_POINT_2F leftWing = D2D1::Point2F(x - size * 0.6f, y - size * 0.2f);  // 왼쪽 날개 끝
		D2D1_POINT_2F rightWing = D2D1::Point2F(x + size * 0.6f, y - size * 0.2f); // 오른쪽 날개 끝

		renderTarget->DrawLine(bottom, top, arrowBrush, 4.0f);      // 기둥
		renderTarget->DrawLine(top, leftWing, arrowBrush, 4.0f);    // 왼쪽 꺾임
		renderTarget->DrawLine(top, rightWing, arrowBrush, 4.0f);   // 오른쪽 꺾임

		// 5. 브러시 해제
		arrowBrush->Release();
	}
}

void EndPoint::OnCollision(GameObject* other)
{
	if (other->GetObjectType() == OBJECTTYPE::PLAYER)
	{
		Scene* currentScene = GET_SINGLE(SceneManager)->GetCurrentScene();

		if (currentScene != nullptr && currentScene->CheckSceneCleared())
		{
			GET_SINGLE(SceneManager)->RequestMapChange("");
		}
		else
		{
			// 클리어되지 않았을 때의 처리 (선택 사항)
			// 예: 화면에 "적을 모두 제거하십시오" 텍스트 띄우기
		}
	}
}

void EndPoint::SaveToData(ObjectSpawnData& outData)
{
	GameObject::SaveToData(outData);
	outData.layers = Layers::ACTORS;
	outData.textData = _targetMapName;
}

void EndPoint::LoadFromData(const ObjectSpawnData& spawnData)
{
	GameObject::LoadFromData(spawnData);
	layer = Layers::ACTORS;
	_targetMapName = spawnData.textData;
}
