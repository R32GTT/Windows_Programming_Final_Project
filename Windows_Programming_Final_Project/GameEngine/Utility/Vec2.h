#pragma once

template <typename T>
class Vec2
{
public:
	Vec2() {};
	Vec2(T _x, T _y) :x(_x), y(_y) {};

	Vec2 Abs() {};
	float Angle() {};
	float Dot(Vec2& other) {};
	bool Is_Equal_Approx(Vec2& other) {};


private:
	float x{}, y{};
	bool is_Normalized = false;

};

