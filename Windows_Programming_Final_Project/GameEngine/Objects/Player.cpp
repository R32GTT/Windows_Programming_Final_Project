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
        info.name = L"MainCharAnim_Crowbar_Idle_Auto";
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
        // 시체 상태일 때 데드 애니메이션 재생 로직이 필요하다면 여기에 작성
        PlayAnimation(_anims[(int)AnimType::DEAD]);
        return;
    }

    Vec2F mousePos = GET_SINGLE(InputManager)->GetMousePos();
    Vec2F dirToMouse = mousePos - GET_SINGLE(SceneManager)->ToRenderPos(pos);

    if (dirToMouse.LengthSq() > 0.0f) {
        facingDir = dirToMouse.Normalized();
        _rotationAngle = facingDir.Angle() * (180.0f / PI) + 90.0f; // Sprite가 위를 향하기 때문에 +90
    }

    // 3. 들고 있는 무기(WPTYPE)에 따라 재생할 애니메이션 세팅 (캐싱된 배열 사용)
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
        if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::LeftMouse))
        {
            status = PlayerState::ATTACK;
            _projectileSpawned = false;
            break; 
        }


        Move();


        if (movingDir.LengthSq() > 0.0f)
        {
            status = PlayerState::MOVE;
            PlayAnimation(_anims[(int)targetMove]);
        }
        else
        {
            status = PlayerState::IDLE;
            PlayAnimation(_anims[(int)targetIdle]);
        }
        break;
    }

    case PlayerState::ATTACK:
    {
        Move();

        PlayAnimation(_anims[(int)targetAttack]);


        int targetFrame = 1; //타격이 들어가는 프레임 번호
        if (_currFrame == targetFrame && !_projectileSpawned)
        {
            _projectileSpawned = true;

            // TODO: 투사체(Projectile) 또는 근접 히트박스 생성 로직 작성
        }

        // [크로우바 공격 캔슬 로직]
        if (currentWeapon_Player == WPTYPE::CROWBAR)
        {
            // 마우스를 떼는 순간 공격 강제 종료 (캔슬)
            if (GET_SINGLE(InputManager)->GetButtonUp(KeyType::LeftMouse))
            {
                status = (movingDir.LengthSq() > 0.0f) ? PlayerState::MOVE : PlayerState::IDLE;
                break;
            }
        }

        // [애니메이션 정상 종료 체크]
        if (_currAnim != nullptr)
        {
            int maxFrame = _currAnim->GetInfo().frames.size() - 1;
            // 애니메이션이 마지막 프레임에 도달했다면 (끝까지 다 휘둘렀다면)
            if (_currFrame >= maxFrame)
            {
                status = (movingDir.LengthSq() > 0.0f) ? PlayerState::MOVE : PlayerState::IDLE;
            }
        }
        break;
    }

    case PlayerState::EXECUTE:
        // 처형 
        break;
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
    {
        Enemy* enemy = static_cast<Enemy*>(other);
        if (enemy->GetEnemyState() == EnemyState::UNCONSCIOUS)
        {
            if (GET_SINGLE(InputManager)->GetButtonDown(KeyType::SpaceBar) && !_isAttacking);
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
