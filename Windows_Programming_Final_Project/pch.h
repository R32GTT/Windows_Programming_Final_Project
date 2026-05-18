#pragma once

const int WinSizeX = 1280;
const int WinSizeY = 720;

const float PI = 3.1415926f;

#include <Windows.h>
#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <string>
#include <algorithm>
#include <functional>


#include <format>
#include <filesystem>
namespace fs = std::filesystem;

#pragma comment(lib, "msimg32.lib")

#define DECLARE_SINGLE(classname)			\
private:									\
	classname() { }							\
public:										\
	static classname* GetInstance()			\
	{										\
		static classname s_instance;		\
		return &s_instance;					\
	}

#define GET_SINGLE(classname)	classname::GetInstance()

#define SAFE_DELETE(ptr)		\
	if (ptr)					\
	{							\
		delete ptr;				\
		ptr = nullptr;			\
	}