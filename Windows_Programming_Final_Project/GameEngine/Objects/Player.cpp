#include "pch.h"
#include "Player.h"
#include "Managers.h"

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

void Player::Render(HDC hdc, float alpha)
{
    // 1. 카메라의 현재 위치를 매니저에서 가져옵니다.
    Vec2<float> camPos = GET_SINGLE(SceneManager)->GetCameraPos();

    // 2. 내 진짜 좌표(GetRenderPos)에서 카메라 좌표를 빼고, 화면 중앙 보정을 더합니다.
    Vec2<float> screenPos = GetRenderPos(alpha);

    int renderX = static_cast<int>(screenPos.x - camPos.x + WinSizeX / 2);
    int renderY = static_cast<int>(screenPos.y - camPos.y + WinSizeY / 2);

    // 3. 이제 원래 그리던 GDI 함수(Rectangle, Ellipse, BitBlt 등)에 
    // 기존 pos.x, pos.y 대신 'renderX', 'renderY'를 대입해서 그립니다!
    // 예시:
    ::Ellipse(hdc, renderX - 20, renderY - 20, renderX + 20, renderY + 20);
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
