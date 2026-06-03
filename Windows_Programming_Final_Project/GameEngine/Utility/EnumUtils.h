#pragma once
#include <string>
#include "Enums.h"

std::string ObjectTypeToString(OBJECTTYPE type);
OBJECTTYPE StringToObjectType(const std::string& str);

std::string LayerToString(Layers layers);
Layers StringToLayer(const std::string& str);



