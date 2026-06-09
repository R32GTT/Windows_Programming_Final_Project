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


void Player::SetWeaponType(WPTYPE type)
{
    currentWeapon_Player = type;
}

WPTYPE Player::GetWeaponType() const
{
    return currentWeapon_Player;
}

void Player::Init()
{
    FlipBook* idleAnim = GET_SINGLE(FileManager)->GetFlipBook(L"MainCharAnim_Idle");
    PlayAnimation(idleAnim);
}

void Player::Update()
{
	SavePrevPos();
    if (IsKilled() && !GET_SINGLE(InputManager)->GetButtonDown(KeyType::R)) return;
	Move();

    Vec2F mousePos = GET_SINGLE(InputManager)->GetMousePos();
    Vec2F dirToMouse = mousePos - GET_SINGLE(SceneManager)->ToRenderPos(pos);

    if (dirToMouse.LengthSq() > 0.0f) {
        facingDir = dirToMouse.Normalized();
        _rotationAngle = facingDir.Angle() * (180.0f / PI) + 90.0f; // Spirte가 위를 향하기 때문에 +90
    }

    //TODO 근접 및 원거리 공격시 애니메이션 추가해야 함.
    // 2. [애니메이션 상태 머신 해결]
    FlipBook* punchAnim = GET_SINGLE(FileManager)->GetFlipBook(L"MainCharAnim_Punch");
    FlipBook* idleAnim = GET_SINGLE(FileManager)->GetFlipBook(L"MainCharAnim_Idle");

    // 마우스를 클릭했고, 현재 공격 중이 아니라면 공격 시작!
    // (InputManager에 GetButtonDown(한번 눌림 체크)가 있다면 그걸 쓰는게 더 좋습니다)
    if (GET_SINGLE(InputManager)->GetButton(KeyType::LeftMouse) && !_isAttacking)
    {
        _isAttacking = true;
        PlayAnimation(punchAnim);
    }

    // 공격 중일 때의 처리 (애니메이션이 끝났는지 확인)
    if (_isAttacking)
    {
        // 현재 펀치 애니메이션이고, 마지막 프레임에 도달했다면 공격 종료!
        if (_currAnim == punchAnim && _currFrame >= punchAnim->GetInfo().frames.size() - 1)
        {
            _isAttacking = false;
        }
    }

    // 공격 중이 아닐 때는 대기(Idle) 모션 유지
    if (!_isAttacking)
    {
        PlayAnimation(idleAnim);
    }

    // 애니메이션 시간 업데이트
    float fixedDt = 1.0f / 60.0f;
    UpdateAnimation(fixedDt);
}

void Player::Render(ID2D1RenderTarget* renderTarget, float alpha)
{
    if (!renderTarget) return;

    Vec2F screenPos = GetRenderPos(alpha);

    Vec2F ToRenderPos = GET_SINGLE(SceneManager)->ToRenderPos(screenPos);

    RenderAnimation(renderTarget, ToRenderPos.x, ToRenderPos.y);
}

bool Player::IsKilled()
{
    return status == PlayerState::DEAD;
}

void Player::OnCollision(GameObject* other)
{
    if (IsKilled()) return;

    switch (other->GetObjectType())
    {
    case OBJECTTYPE::PROJECTILE:
        status = PlayerState::DEAD;
        break;
    case OBJECTTYPE::ENEMY:
        if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::SpaceBar) && !_isAttacking);
        break;
    case OBJECTTYPE::ENDPOINT:
        break;
    default:
            break;
    }
}

void Player::SaveToData(ObjectSpawnData& outData)
{
    GameObject::SaveToData(outData);

}

void Player::LoadFromData(const ObjectSpawnData& spawnData)
{
    GameObject::LoadFromData(spawnData);
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
