#include "pch.h"
#include "EnumUtils.h"

std::string ObjectTypeToString(OBJECTTYPE type)
{
    switch (type)
    {
    case OBJECTTYPE::NONE:         return "NONE";
    case OBJECTTYPE::PLAYER:       return "PLAYER";
    case OBJECTTYPE::ENEMY:        return "ENEMY";
    case OBJECTTYPE::WEAPON:       return "WEAPON";
    case OBJECTTYPE::PROJECTILE:   return "PROJECTILE";
    case OBJECTTYPE::DECO:         return "DECO";
    case OBJECTTYPE::WALL:         return "WALL";
    case OBJECTTYPE::FLOOR:        return "FLOOR";
    case OBJECTTYPE::ENDPOINT:     return "ENDPOINT";
    case OBJECTTYPE::OBJECT_COUNT: return "OBJECT_COUNT";
    default:                       return "NONE";
    }
}

OBJECTTYPE StringToObjectType(const std::string& str)
{
    static const std::unordered_map<std::string, OBJECTTYPE> map = {
        {"NONE",         OBJECTTYPE::NONE},
        {"PLAYER",       OBJECTTYPE::PLAYER},
        {"ENEMY",        OBJECTTYPE::ENEMY},
        {"WEAPON",       OBJECTTYPE::WEAPON},
        {"PROJECTILE",   OBJECTTYPE::PROJECTILE},
        {"DECO",         OBJECTTYPE::DECO},
        {"WALL",         OBJECTTYPE::WALL},
        {"FLOOR",        OBJECTTYPE::FLOOR},
        {"ENDPOINT",     OBJECTTYPE::ENDPOINT},
        {"OBJECT_COUNT", OBJECTTYPE::OBJECT_COUNT}
    };

    auto it = map.find(str);
    if (it != map.end())
        return it->second;

    return OBJECTTYPE::NONE;
}

std::string LayerToString(Layers layers)
{
    switch (layers)
    {
    case Layers::FLOOR:       return "FLOOR";
    case Layers::WALL:        return "WALL";
    case Layers::DECO:        return "DECO";
    case Layers::ITEM:        return "ITEM";
    case Layers::ACTORS:      return "ACTORS";
    case Layers::UI:          return "UI";
    case Layers::LAYER_COUNT: return "LAYER_COUNT";
    default:                  return "FLOOR";
    }
}

Layers StringToLayer(const std::string& str)
{
    static const std::unordered_map<std::string, Layers> map = {
        {"FLOOR",       Layers::FLOOR},
        {"WALL",        Layers::WALL},
        {"DECO",        Layers::DECO},
        {"ITEM",        Layers::ITEM},
        {"ACTORS",      Layers::ACTORS},
        {"UI",          Layers::UI},
        {"LAYER_COUNT", Layers::LAYER_COUNT}
    };

    auto it = map.find(str);
    if (it != map.end())
        return it->second;

    return Layers::FLOOR;
}

std::string EnemyTypeToString(EnemyType type)
{
    switch (type)
    {
    case EnemyType::NORMAL:  return "NORMAL";
    case EnemyType::ARMORED: return "ARMORED";
    default:                 return "NORMAL";
    }
}

EnemyType StringToEnemyType(const std::string& str)
{
    static const std::unordered_map<std::string, EnemyType> map = {
        {"NORMAL",  EnemyType::NORMAL},
        {"ARMORED", EnemyType::ARMORED}
    };

    auto it = map.find(str);
    if (it != map.end())
        return it->second;

    return EnemyType::NORMAL;
}

std::string WeaponTypeToString(WPTYPE type)
{
    switch (type)
    {
    case WPTYPE::NONE:        return "NONE";
    case WPTYPE::FIST:        return "FIST";
    case WPTYPE::CROWBAR:     return "CROWBAR";
    case WPTYPE::RIFLE:       return "RIFLE";
    case WPTYPE::RANDOM_ANY:   return "RANDOM_ANY";
    case WPTYPE::TOTAL_COUNT: return "TOTAL_COUNT";
    default:                  return "NONE";
    }
}

WPTYPE StringToWeaponType(const std::string& str)
{
    static const std::unordered_map<std::string, WPTYPE> map = {
        {"NONE",        WPTYPE::NONE},
        {"FIST",        WPTYPE::FIST},
        {"CROWBAR",     WPTYPE::CROWBAR},
        {"RIFLE",       WPTYPE::RIFLE},
        {"RANDOM_ANY", WPTYPE::RANDOM_ANY},
        {"TOTAL_COUNT", WPTYPE::TOTAL_COUNT}
    };

    auto it = map.find(str);
    if (it != map.end())
        return it->second;

    return WPTYPE::NONE;
}

std::string EnemyStateToString(EnemyState type)
{
    switch (type)
    {
    case EnemyState::IDLE:
        return "IDLE";
        break;
    case EnemyState::PATROL:
        return "PATROL";
        break;
    case EnemyState::ROAMING:
        return "ROAMING";
        break;
    case EnemyState::CHASE:
        return "CHASE";
        break;
    case EnemyState::ATTACK:
        return "ATTACK";
        break;
    case EnemyState::UNCONSCIOUS:
        return "UNCONSCIOUS";
        break;
    case EnemyState::DEAD:
        return "DEAD";
        break;
    default:
        return "IDLE";
        break;
    }
}

EnemyState StringToEnemyState(const std::string& str)
{
    static const std::unordered_map<std::string, EnemyState> map = {
        {"IDLE",        EnemyState::IDLE},
        {"PATROL",        EnemyState::PATROL},
        {"ROAMING",     EnemyState::ROAMING},
        {"CHASE",       EnemyState::CHASE},
        {"ATTACK", EnemyState::ATTACK},
        {"UNCONSCIOUS", EnemyState::UNCONSCIOUS},
        {"DEAD", EnemyState::DEAD}
    };

    auto it = map.find(str);
    if (it != map.end())
        return it->second;

    return EnemyState::IDLE;
}
