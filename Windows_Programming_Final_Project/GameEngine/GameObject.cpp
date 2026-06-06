#include "pch.h"
#include "GameObject.h"
#include "Managers.h"
#include "FileBase/FileTypes/FlipBook.h"
#include "FileBase/FileTypes/Sprite.h"
#include "LevelData/LevelData.h"

unsigned int GameObject::_sNextId = 1;

GameObject::GameObject()
{
	_id = _sNextId++;

	//오브젝트 고유 번호는 있으니 필요한 것들을 추가함.
	//지워도 괜찮음
	pos = { 0.0f, 0.0f };
	prevPos = pos;

	facingDir = { 0.0f, -1.0f };  //기본적으로 오른쪽을 바라봅니다(위쪽 방향을 기본 방향으로 할 시 0.0f -1.0f로 한다)
	movingDir = { 0.0f, 0.0f };
	_halfSize = { 25.0f, 25.0f };
	speed = 0.0f;


}

GameObject::~GameObject()
{
}

void GameObject::Init()
{
}

void GameObject::Update()
{
}

void GameObject::Render(ID2D1RenderTarget* renderTarget, float alpha)
{

}


bool GameObject::CheckDead()
{
	return false;
}

Vec2<float> GameObject::GetRenderPos(float alpha)
{
	return prevPos * (1.0f - alpha) + pos * alpha;
}

// 현재 사용 안함.
//Vec2F GameObject::GetScreenPos(float alpha)
//{
//    Vec2F worldPos = GetRenderPos(alpha);
//
//    Vec2F camPos = GET_SINGLE(SceneManager)->GetCameraPos();
//
//	Vec2F WinSizeV(WinSizeX, WinSizeY);
//	Vec2F screen = worldPos - camPos + WinSizeV / 2.0f;
//
//    /*int screenX = std::round(worldPos.x - camPos.x + WinSizeX / 2.0f);
//    int screenY = std::round(worldPos.y - camPos.y + WinSizeY / 2.0f);
//	*/
//
//    return screen;
//}

void GameObject::PlayAnimation(FlipBook* anim)
{
	if (anim == nullptr || _currAnim == anim)  return;

	_currAnim = anim;
	_currFrame = 0;
	_animTimer = 0.0f;
}

void GameObject::UpdateAnimation(float dt)
{
	if (_currAnim == nullptr) return;

	const FlipbookInfo& info = _currAnim->GetInfo();
	if (info.frames.empty()) return;

	_animTimer += dt;

	if (_animTimer >= info.duration)
	{
		_animTimer -= info.duration;
		_currFrame++;

		if (_currFrame >= info.frames.size())
		{
			if (info.loop)
				_currFrame = 0;
			else
				_currFrame = (int)info.frames.size() - 1;
		}
	}
}

void GameObject::RenderAnimation(ID2D1RenderTarget* renderTarget, float renderX, float renderY)
{
	if (_currAnim == nullptr || _currAnim->GetInfo().frames.empty()) return;

	Sprite* currentSprite = _currAnim->GetInfo().frames[_currFrame];
	ID2D1Bitmap* dxBitmap = currentSprite->GetBitmap();
	D2D1_RECT_F srcRect = currentSprite->GetSrcRect();

	Vec2F spriteSize = currentSprite->GetSize();

	float destLeft = renderX - (spriteSize.x / 2.0f) + _renderOffset.x;
	float destTop = renderY - (spriteSize.y / 2.0f) + _renderOffset.y;
	float destRight = destLeft + spriteSize.x;
	float destBottom = destTop + spriteSize.y;

	D2D1_RECT_F destRect = D2D1::RectF(destLeft, destTop, destRight, destBottom);

	D2D1_MATRIX_3X2_F oldTransform;
	renderTarget->GetTransform(&oldTransform);

	D2D1_POINT_2F pivotPoint = D2D1::Point2F(renderX, renderY);

	renderTarget->SetTransform(
		D2D1::Matrix3x2F::Rotation(_rotationAngle, pivotPoint) * oldTransform
	);

	renderTarget->DrawBitmap(dxBitmap, &destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, &srcRect);

	renderTarget->SetTransform(oldTransform);
}



void GameObject::SaveToData(ObjectSpawnData& outData)
{
	outData.fileID = GetID();
	outData.layers = GetLayer();
	outData.objType = GetObjectType();
	outData.pos = GetPos();
	outData.halfSize = GetHalfSize();
	outData.spriteName = GetSpriteName();
}

void GameObject::LoadFromData(const ObjectSpawnData& spawnData)
{
	SetPos(spawnData.pos);
	prevPos = spawnData.pos;
	SetHalfSize(spawnData.halfSize);
	layer = (spawnData.layers);
	SetSpriteName(spawnData.spriteName);
}



