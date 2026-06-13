#pragma once
#include <string>
#include "Enums.h"

std::string ObjectTypeToString(OBJECTTYPE type);
OBJECTTYPE StringToObjectType(const std::string& str);

std::string LayerToString(Layers layers);
Layers StringToLayer(const std::string& str);

std::string EnemyTypeToString(EnemyType type);
EnemyType StringToEnemyType(const std::string& str);

std::string WeaponTypeToString(WPTYPE type);
WPTYPE StringToWeaponType(const std::string& str);

std::string EnemyStateToString(EnemyState type);
EnemyState StringToEnemyState(const std::string& str);

