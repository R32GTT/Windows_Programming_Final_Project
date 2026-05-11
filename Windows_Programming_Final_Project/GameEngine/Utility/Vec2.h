#pragma once

template <typename T>
class Vec2
{
public:
	Vec2() {};
	Vec2(T _x, T _y) :x(_x), y(_y) {};



private:
	float x{}, y{};
	
};

