#include "pch.h"
#include "Player.h"
#include "Managers.h"
#include "../FileBase/FileTypes/Texture.h"
#include "../FileBase/FileTypes/FlipBook.h"

Player::Player()
{
    speed = playerSpeed;
    _halfSize = Vec2F(20.0f, 20.0f);

    type = OBJECTTYPE::PLAYER;
    layer = Layers::ACTORS;
}

Player::~Player()
{
}

void Player::Init()
{
}

void Player::Update()
{
	SavePrevPos();

	Move();

    

	Vec2<float> mousePos = GET_SINGLE(InputManager)->GetMousePos();
	Vec2<float> dirToMouse = mousePos - pos;

	if (dirToMouse.LengthSq() > 0.0f) {
		facingDir = dirToMouse.Normalized();
        _rotationAngle = facingDir.Angle() * (180.0f / PI);
	}

    if (_currAnim)
    {
        _renderOffset = Vec2F(0.0f, 32.0f);
    }

    float fixedDT = 1.0f / 60.0f;
    UpdateAnimation(fixedDT);
}

void Player::Render(ID2D1RenderTarget* renderTarget, float alpha)
{
    if (!renderTarget) return;

    Vec2<float> camPos = GET_SINGLE(SceneManager)->GetCameraPos();
    Vec2<float> screenPos = GetRenderPos(alpha);

    float renderX = screenPos.x - camPos.x + WinSizeX / 2.0f;
    float renderY = screenPos.y - camPos.y + WinSizeY / 2.0f;

    RenderAnimation(renderTarget, renderX, renderY);

    /*Texture* playerTex = GET_SINGLE(FileManager)->GetTexture(L"Player");

    if (playerTex == nullptr || playerTex->GetBitmap() == nullptr)
    {
        ID2D1SolidColorBrush* temporaryBrush = nullptr;
        renderTarget->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &temporaryBrush);
        if (temporaryBrush)
        {
            D2D1_ELLIPSE ellipse = D2D1::Ellipse(D2D1::Point2F(renderX, renderY), _halfSize.x, _halfSize.y);
            renderTarget->FillEllipse(ellipse, temporaryBrush);
            temporaryBrush->Release();
        }
        return;
    }*/

    //// 3. 로드된 DX2D 비트맵 객체 획득
    //ID2D1Bitmap* pBitmap = playerTex->GetBitmap();

    //// 4. 그려질 목적지 사각형 구역 설정 (중심점 기준)
    //D2D1_RECT_F destRect = D2D1::RectF(
    //    renderX - _halfSize.x,
    //    renderY - _halfSize.y,
    //    renderX + _halfSize.x,
    //    renderY + _halfSize.y
    //);

 
    //renderTarget->DrawBitmap(
    //    pBitmap,
    //    &destRect,
    //    1.0f, // 투명도 (Alpha)
    //    D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
    //    nullptr // 원본 소스 사각형 구역 (전체 출력 시 nullptr)
    //);
}

PlayerState Player::Move()
{
    movingDir.x = 0.0f;
    movingDir.y = 0.0f;

    InputManager* input = GET_SINGLE(InputManager);

    if (input->GetButton(KeyType::W)) 
        movingDir.y -= 1.0f;
    if (input->GetButton(KeyType::S)) movingDir.y += 1.0f;
    if (input->GetButton(KeyType::A)) movingDir.x -= 1.0f;
    if (input->GetButton(KeyType::D)) movingDir.x += 1.0f;

    if (movingDir.LengthSq() > 0.0f) {
        movingDir = movingDir.Normalized();

        pos = pos + (movingDir * speed);
        status = PlayerState::MOVE;
    }
    else {
        status = PlayerState::IDLE;
    }

    return status;
}
