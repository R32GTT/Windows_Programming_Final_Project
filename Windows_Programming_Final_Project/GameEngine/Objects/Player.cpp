#include "pch.h"
#include "Player.h"
#include "Weapon.h" //Weapon 헤더 포함
#include "Managers.h"
#include "Enemy.h"
#include "../FileBase/FileTypes/Texture.h"
#include "../FileBase/FileTypes/FlipBook.h"

Player::Player()
{
    speed = playerSpeed;
    _halfSize = Vec2F(20.0f, 20.0f);

    type = OBJECTTYPE::PLAYER;
    layer = Layers::ACTORS;
    currentWeapon_Player = WPTYPE::CROWBAR;
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

//무기 장착 함수들 구현
void Player::EquipWeapon(Weapon* weapon)
{
    if (weapon == nullptr) return;

    if (currentWeapon != nullptr) {
        DropWeapon();
    }

    currentWeapon = weapon;
    currentWeapon->SetOwner(this);

    is_Item = true;
    is_drop = false;
}

//무기 버리기 구현
void Player::DropWeapon()
{
    if (currentWeapon != nullptr) {
        currentWeapon->SetOwner(nullptr);
        currentWeapon = nullptr;
    }

    is_Item = false;
    is_drop = true;
    currentWeapon_Player = WPTYPE::NONE;
}


void Player::Init()
{
    //FlipBook* idleAnim = GET_SINGLE(FileManager)->GetFlipBook(L"MainCharAnim_Idle");
    //PlayAnimation(idleAnim);

    FileManager* FM = GET_SINGLE(FileManager);

    _anims[(int)AnimType::IDLE] = FM->GetFlipBook(L"MainCharAnim_Idle");
    _anims[(int)AnimType::MOVE] = FM->GetFlipBook(L"MainCharAnim_Walking");
    _anims[(int)AnimType::ATTACK_FIST] = FM->GetFlipBook(L"MainCharAnim_Punch");
    _anims[(int)AnimType::ATTACK_CROWBAR] = FM->GetFlipBook(L"MainCharAnim_Crowbar");
    _anims[(int)AnimType::ATTACK_GUN] = FM->GetFlipBook(L"MainCharAnim_Rifle Shooting");
    _anims[(int)AnimType::EXECUTE] = FM->GetFlipBook(L"MainCharAnim_Execution");
    _anims[(int)AnimType::DEAD] = FM->GetFlipBook(L"MainCharAnim_Dead");
    //_anims[(int)AnimType::UNCONSCIOUS] = FM->GetFlipBook(L"MainCharAnim_Unconscious"); Mob 애들만 필요해서 굳이?

    _anims[(int)AnimType::IDLE_CROWBAR] = FM->CreateFlipBook(L"MainCharAnim_Crowbar_Idle_Auto");
    if (_anims[(int)AnimType::IDLE_CROWBAR]->GetInfo().frames.empty() && _anims[(int)AnimType::ATTACK_CROWBAR] != nullptr)
    {
        FlipbookInfo info;
        info.name = L"MainCharAnim_Crowbar_Idle_Auto";
        info.frames.push_back(_anims[(int)AnimType::ATTACK_CROWBAR]->GetInfo().frames[0]);
        info.duration = 1.0f;
        info.loop = true;
        _anims[(int)AnimType::IDLE_CROWBAR]->SetInfo(info);
    }

    _anims[(int)AnimType::IDLE_GUN] = FM->CreateFlipBook(L"MainCharAnim_Gun_Idle_Auto");
    if (_anims[(int)AnimType::IDLE_GUN]->GetInfo().frames.empty() && _anims[(int)AnimType::ATTACK_GUN] != nullptr)
    {
        FlipbookInfo info;
        info.name = L"MainCharAnim_Gun_Idle_Auto";
        info.frames.push_back(_anims[(int)AnimType::ATTACK_GUN]->GetInfo().frames[0]);
        info.duration = 1.0f;
        info.loop = true;
        _anims[(int)AnimType::IDLE_GUN]->SetInfo(info);
    }
}

void Player::Update()
{
    SavePrevPos();

    if (IsKilled())
    {
        PlayAnimation(_anims[(int)AnimType::DEAD]);
        return;
    }

    Vec2F mousePos = GET_SINGLE(InputManager)->GetMousePos();
    Vec2F dirToMouse = mousePos - GET_SINGLE(SceneManager)->ToRenderPos(pos);

    if (dirToMouse.LengthSq() > 0.0f) {
        facingDir = dirToMouse.Normalized();
        _rotationAngle = facingDir.Angle() * (180.0f / PI) + 90.0f; // Sprite가 위를 향하기 때문에 +90
    }


    Move();


    AnimType targetIdle = AnimType::IDLE;
    AnimType targetMove = AnimType::MOVE;
    AnimType targetAttack = AnimType::ATTACK_FIST;

    if (currentWeapon_Player == WPTYPE::CROWBAR)
    {
        targetIdle = AnimType::IDLE_CROWBAR;
        targetMove = AnimType::IDLE_CROWBAR;
        targetAttack = AnimType::ATTACK_CROWBAR;
    }
    else if (currentWeapon_Player == WPTYPE::RIFLE)
    {
        targetIdle = AnimType::IDLE_GUN;
        targetMove = AnimType::IDLE_GUN;
        targetAttack = AnimType::ATTACK_GUN;
    }

    switch (status)
    {
    case PlayerState::IDLE:
    case PlayerState::MOVE:
    {
        bool isAttackTriggered = false;

        if (currentWeapon_Player == WPTYPE::RIFLE) {
            isAttackTriggered = GET_SINGLE(InputManager)->GetButton(KeyType::LeftMouse);
        }
        else {
            isAttackTriggered = GET_SINGLE(InputManager)->GetButton(KeyType::LeftMouse);
        }

        if (isAttackTriggered)
        {
            status = PlayerState::ATTACK;
            _projectileSpawned = false;


            _currFrame = 0;
            _animTimer = 0.0f;

            break;
        }

        // 공격 중이 아닐 때만, 이동 여부에 따라 MOVE / IDLE 애니메이션 재생
        if (movingDir.LengthSq() > 0.0f) {
            status = PlayerState::MOVE;
            PlayAnimation(_anims[(int)targetMove]);
        }
        else {
            status = PlayerState::IDLE;
            PlayAnimation(_anims[(int)targetIdle]);
        }
        break;
    }
    case PlayerState::ATTACK:
    {
        PlayAnimation(_anims[(int)targetAttack]);


        int targetFrame = 2; // (수정 필요) 실제로 총알이 나가거나 빠루 타격이 들어가는 프레임 번호
        if (_currFrame == targetFrame && !_projectileSpawned)
        {
            _projectileSpawned = true;
            /* TODO

            // 1. 투사체 객체 생성
            Projectile* proj = new Projectile();
            proj->Init();

            // 2. 발사 방향 및 위치 세팅 (플레이어 위치 + 바라보는 방향 * 총구 거리)
            proj->SetDirection(facingDir);
            proj->SetPos(pos + facingDir * 25.0f);

            // 3. 투사체 정보 세팅 (데미지, 스피드 등 Projectile.h의 SetInfo 활용)
            // proj->SetInfo(this, Projectile::ProjectileType::BULLET, 10.0f, 800.0f, 2.0f);

            // 4. 씬 매니저에 투사체 등록 (화면에 렌더링되고 Update를 타게 됨)
            GET_SINGLE(SceneManager)->GetCurrentScene()->AddObject(proj);

            */
        }


        if (currentWeapon_Player == WPTYPE::CROWBAR)
        {

            int midFrame = 4;

            if (_currFrame >= midFrame && !GET_SINGLE(InputManager)->GetButton(KeyType::LeftMouse))
            {

                status = (movingDir.LengthSq() > 0.0f) ? PlayerState::MOVE : PlayerState::IDLE;
                break;
            }
        }


        if (_currAnim != nullptr && _currAnim == _anims[(int)targetAttack])
        {
            int maxFrame = _currAnim->GetInfo().frames.size() - 1;
            // 끝까지 재생되었다면
            if (_currFrame >= maxFrame)
            {
                // 공격이 끝난 시점에 계속 걷고 있다면 MOVE로, 멈췄다면 IDLE로 복귀
                status = (movingDir.LengthSq() > 0.0f) ? PlayerState::MOVE : PlayerState::IDLE;
            }
        }
        break;
    }
    case PlayerState::EXECUTE:
        // 처형 로직 등...
        break;
    }

    UpdateAnimation(1.0f / 60.0f);
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
    {
        Enemy* enemy = static_cast<Enemy*>(other);
        if (enemy->GetEnemyState() == EnemyState::UNCONSCIOUS)
        {
            if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::SpaceBar) && !_isAttacking)
            {
                status = PlayerState::EXECUTE;
                SetPos(enemy->GetPos());
                //TODO

            }
        }
        break;
    }
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

void Player::Move()
{
    movingDir = { 0.0f, 0.0f };
    InputManager* input = GET_SINGLE(InputManager);

    if (input->GetButton(KeyType::W)) movingDir.y -= 1.0f;
    if (input->GetButton(KeyType::S)) movingDir.y += 1.0f;
    if (input->GetButton(KeyType::A)) movingDir.x -= 1.0f;
    if (input->GetButton(KeyType::D)) movingDir.x += 1.0f;

    if (movingDir.LengthSq() > 0.0f)
    {
        movingDir = movingDir.Normalized();
        pos = pos + movingDir * speed;
    }
}
