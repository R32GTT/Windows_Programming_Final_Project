#include "pch.h"
#include "Player.h"
#include "Managers.h"
#include "../FileBase/FileTypes/Texture.h"

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
	}
}

void Player::Render(ID2D1RenderTarget* renderTarget, float alpha)
{
    if (!renderTarget) return;

    // 1. 카메라 및 화면 중앙 보정이 들어간 좌표 계산 (기존 로직 유지)
    Vec2<float> camPos = GET_SINGLE(SceneManager)->GetCameraPos();
    Vec2<float> screenPos = GetRenderPos(alpha);

    float renderX = screenPos.x - camPos.x + WinSizeX / 2.0f;
    float renderY = screenPos.y - camPos.y + WinSizeY / 2.0f;

    // 2. FileManager에서 등록해 둔 플레이어 텍스처를 꺼내옴
    // (예시로 씬 진입이나 Init 단계에서 LoadTexture(L"Player", L"Player.png")가 끝났다고 가정)
    Texture* playerTex = GET_SINGLE(FileManager)->GetTexture(L"Player");

    // 만약 텍스처가 아직 없거나 비트맵 로드가 안 되었다면 임시로 원이라도 그리게 방어코드 작성
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
    }

    // 3. 로드된 DX2D 비트맵 객체 획득
    ID2D1Bitmap* pBitmap = playerTex->GetBitmap();

    // 4. 그려질 목적지 사각형 구역 설정 (중심점 기준)
    D2D1_RECT_F destRect = D2D1::RectF(
        renderX - _halfSize.x,
        renderY - _halfSize.y,
        renderX + _halfSize.x,
        renderY + _halfSize.y
    );

    // 5. 비트맵 그리기 수행
    // srcRect 자리에 nullptr을 주면 이미지 전체를 원본 사이즈로 destRect에 맞춰 그라데이션 보정해서 그려줌
    renderTarget->DrawBitmap(
        pBitmap,
        &destRect,
        1.0f, // 투명도 (Alpha)
        D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
        nullptr // 원본 소스 사각형 구역 (전체 출력 시 nullptr)
    );
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
