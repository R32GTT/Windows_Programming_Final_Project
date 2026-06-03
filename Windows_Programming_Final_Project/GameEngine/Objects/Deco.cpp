#include "pch.h"
#include "Deco.h"


DECO::~DECO()
{
}

//_halfSize는 wall과 일단 똑같이 25 25
void DECO::Init()
{
	layer = Layers::DECO;
}

void DECO::Update()
{


}

void DECO::Render(ID2D1RenderTarget* renderTarget, float alpha)
{
	if (renderTarget == nullptr) return;

	Vec2<float> renderPos = GetRenderPos(alpha);

	D2D1_RECT_F rect = D2D1::RectF(renderPos.x - _halfSize.x, renderPos.y - _halfSize.y, renderPos.x + _halfSize.x, renderPos.y + _halfSize.y);

	ID2D1SolidColorBrush* brush = nullptr;

	renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::LightGreen), &brush);

	if (brush != nullptr)
	{
		renderTarget->FillRectangle(rect, brush);
		brush->Release();
	}


}
