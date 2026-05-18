#pragma once
#include <cmath>
#include "../../pch.h"

template <typename T>
class Vec2
{
public:
	Vec2() {};
	Vec2(T _x, T _y) :x(_x), y(_y) {};
	Vec2(Vec2& base) { x = base.x; y = base.y; };
	Vec2(POINT pt) { x = (float)pt.x; y = (float)pt.y; };

	float LengthSq();
	float Length();

	Vec2<T> Abs();
	Vec2<T> Normalized();
	Vec2<T> Reflect(const Vec2& other);

	
	float Angle();
	float Dot(Vec2& other);
	float Aspect() { return x / y; };
	float Cross(Vec2& other);

	bool Is_Equal_Approx(Vec2& other);
	bool is_Normalized();


	Vec2 operator+(Vec2 rhs) { return Vec2(x + rhs.x, y + rhs.y); };
	Vec2 operator-(Vec2 rhs) { return Vec2(x - rhs.x, y - rhs.y); };

	Vec2 operator/(Vec2 rhs) { return Vec2(x / rhs.x, y / rhs.y); }; //Vec2로 나누기
	Vec2 operator/(float rhs) { return Vec2(x / rhs, y / rhs); };	 //Float scalar 나누기
	Vec2 operator/(int rhs) { return Vec2(x / rhs, y / rhs); };		 //Int scalar 나누기

	Vec2 operator*(Vec2 rhs) { return Vec2(x * rhs.x, y * rhs.y); }; //Vec2 끼리 곱
	Vec2 operator*(float rhs) { return Vec2(x * rhs, y * rhs); };	 //Float scalar 곱
	Vec2 operator*(int rhs) { return Vec2(x * rhs, y * rhs); };		 //Int scalar 곱

	bool operator<(Vec2 rhs) { return ((x < rhs.x) && (y < rhs.y)); };
	bool operator<=(Vec2 rhs) { return ((x <= rhs.x) && (y <= rhs.y)); };
	bool operator==(Vec2 rhs) { return ((x == rhs.x) && (y == rhs.y)); };
	bool operator>=(Vec2 rhs) { return ((x >= rhs.x) && (y >= rhs.y)); };
	bool operator>(Vec2 rhs) { return ((x > rhs.x) && (y > rhs.y)); };
	T operator[](int index) { if (index > 1 || index < 0) return -1; if (index) return y; else return x; };

	





	T x{}, y{};
	


};


