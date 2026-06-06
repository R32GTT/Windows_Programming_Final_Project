#include "pch.h"
#include "Wall.h"
#include "Managers.h"

Wall::~Wall()
{
}

//wall layer를 수정
//halfsize 일단 25 25로 함
void Wall::Init()
{
	layer = Layers::WALL;

}

void Wall::Update()
{
}

//Wall Render 시작하기
//Sprite가 없으니 일반 도형으로 Render하기
void Wall::Render(ID2D1RenderTarget* renderTarget, float alpha)
{
	if (!renderTarget) return;

	Vec2F topLeftWorld{ wallCoords.left, wallCoords.top };
	Vec2F bottomRightWorld{ wallCoords.right, wallCoords.bottom };

	Vec2F topLeftScreen = GET_SINGLE(SceneManager)->ToRenderPos(topLeftWorld);
	Vec2F bottomRightScreen = GET_SINGLE(SceneManager)->ToRenderPos(bottomRightWorld);

	D2D1_RECT_F rect = D2D1::RectF(topLeftScreen.x, topLeftScreen.y, bottomRightScreen.x, bottomRightScreen.y);

	ID2D1SolidColorBrush* brush = nullptr;
	renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Blue, 0.5f), &brush);

	if (brush != nullptr)
	{
		renderTarget->FillRectangle(rect, brush);

		// 하얀 테두리를 그려주면 에디터에서 식별하기 아주 좋습니다
		ID2D1SolidColorBrush* borderBrush = nullptr;
		renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &borderBrush);
		if (borderBrush) {
			renderTarget->DrawRectangle(rect, borderBrush);
			borderBrush->Release();
		}

		brush->Release();
	}
}

void Wall::SaveToData(ObjectSpawnData& outData)
{
	GameObject::SaveToData(outData);
	outData.WallCoords = wallCoords;
}

void Wall::LoadFromData(const ObjectSpawnData& spawnData)
{
	GameObject::LoadFromData(spawnData);
	wallCoords = spawnData.WallCoords;
}

Wall::Wall(POINT start, POINT end)
{
	type = OBJECTTYPE::WALL;
	layer = Layers::WALL;

	// 드래그 방향에 상관없이 무조건 (left < right), (top < bottom)이 되도록 정렬
	wallCoords.left = (float)(std::min)(start.x, end.x);
	wallCoords.right = (float)(std::max)(start.x, end.x);
	wallCoords.top = (float)(std::min)(start.y, end.y);
	wallCoords.bottom = (float)(std::max)(start.y, end.y);

	// 나중에 우클릭(마우스 피킹)으로 지울 수 있도록 중심점(Pos)과 절반 크기(HalfSize) 계산해서 넣어주기
	float centerX = (wallCoords.left + wallCoords.right) / 2.0f;
	float centerY = (wallCoords.top + wallCoords.bottom) / 2.0f;
	float sizeX = (wallCoords.right - wallCoords.left) / 2.0f;
	float sizeY = (wallCoords.bottom - wallCoords.top) / 2.0f;

	SetPos(Vec2F{ centerX, centerY });
	SetHalfSize(Vec2F{ sizeX, sizeY });

}
