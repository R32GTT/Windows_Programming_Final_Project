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

    
    // 1. [회전 버그 해결] 내 월드 좌표(pos)를 마우스와 같은 '화면 좌표'로 변환!
    Vec2<float> camPos = GET_SINGLE(SceneManager)->GetCameraPos();
    Vec2<float> myScreenPos = pos - camPos + Vec2<float>(WinSizeX / 2.0f, WinSizeY / 2.0f);

    Vec2<float> mousePos = GET_SINGLE(InputManager)->GetMousePos();
    Vec2<float> dirToMouse = mousePos - myScreenPos; // (마우스 화면좌표) - (내 화면좌표)

    if (dirToMouse.LengthSq() > 0.0f) {
        facingDir = dirToMouse.Normalized();
        _rotationAngle = facingDir.Angle() * (180.0f / PI) + 90.0f;
    }

    // 2. [애니메이션 상태 머신 해결]
    FlipBook* punchAnim = GET_SINGLE(FileManager)->GetFlipBook(L"MainCharAnim_Punch");
    FlipBook* idleAnim = GET_SINGLE(FileManager)->GetFlipBook(L"MainCharAnim_Idle");

    // 마우스를 클릭했고, 현재 공격 중이 아니라면 공격 시작!
    // (InputManager에 GetButtonDown(한번 눌림 체크)가 있다면 그걸 쓰는게 더 좋습니다)
    if (GET_SINGLE(InputManager)->GetButton(KeyType::LeftMouse) && !_isAttacking)
    {
        _isAttacking = true;
        PlayAnimation(punchAnim);

        // 3. [피벗 버그 해결] Aseprite에서 무기를 뻗은 방향에 맞게 수정하세요!
        // 오른쪽으로 뻗으면 (32.0f, 0.0f), 왼쪽이면 (-32.0f, 0.0f), 위면 (0.0f, -32.0f)
        _renderOffset = Vec2<float>(0.0f, -32.0f);
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
        _renderOffset = Vec2<float>(0.0f, 0.0f);
    }

    // 애니메이션 시간 업데이트
    float fixedDt = 1.0f / 60.f;
    UpdateAnimation(fixedDt);
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
