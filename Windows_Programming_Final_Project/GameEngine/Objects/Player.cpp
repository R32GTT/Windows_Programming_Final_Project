#include "pch.h"
#include "Player.h"
#include "Managers.h"

Player::Player()
{
    speed = 5.0f;

    type = OBJECTTYPE::PLAYER;
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
    Vec2<float> renderPos = GetRenderPos(alpha);

    // 실제 pos 대신 보간된 renderPos를 사용하여 부드러운 움직임을 연출합니다.
    Ellipse(hdc,
        static_cast<int>(renderPos.x) - 10,
        static_cast<int>(renderPos.y) - 10,
        static_cast<int>(renderPos.x) + 10,
        static_cast<int>(renderPos.y) + 10);
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
