#pragma once
#include <cmath>
#include <cassert>
#include "pch.h"

template <typename T>
class Vec2
{
public:
constexpr	Vec2() {};
constexpr	Vec2(T _x, T _y) :x(_x), y(_y) {};
constexpr	Vec2(const Vec2& base) : x(base.x), y(base.y) {};
constexpr	Vec2(Vec2&& base) noexcept = default;
	Vec2(POINT pt) { x = (float)pt.x; y = (float)pt.y; };

	~Vec2() = default;

	float LengthSq() const;
	float Length() const;

	Vec2<T> Abs();
	Vec2<T> Normalized();
	Vec2<T> Reflect(Vec2& other);

	float Angle();
	float Dot(const Vec2& other);
	float Aspect() { return x / y; };
	float Cross(const Vec2& other);

	bool Is_Equal_Approx(Vec2& other);
	bool is_Normalized();

	Vec2 operator+(const Vec2& rhs)const { return Vec2(x + rhs.x, y + rhs.y); };
	Vec2 operator-(const Vec2& rhs)const  { return Vec2(x - rhs.x, y - rhs.y); };

	Vec2 operator/(const Vec2& rhs)const { return Vec2(x / rhs.x, y / rhs.y); }; //Vec2로 나누기
	Vec2 operator/(T scalar)const { return Vec2(x / scalar, y / scalar); };

	Vec2 operator*(const Vec2& rhs)const { return Vec2(x * rhs.x, y * rhs.y); }; //Vec2 끼리 곱
	Vec2 operator*(T scalar)const { return Vec2(x * scalar, y * scalar); };

	Vec2 operator=(const Vec2& rhs)
	{
		x = rhs.x; y = rhs.y;
		return *this;
	};

	bool operator<(const Vec2& rhs) const
	{
		if (x != rhs.x) return x < rhs.x;
		return y < rhs.y;
	};
	bool operator>(const Vec2& rhs) const { return rhs < *this; };

	bool operator<=(const Vec2 & rhs) const { return !(*this > rhs); };
	bool operator>=(const Vec2 & rhs) const { return !(*this < rhs); };

	T& operator[](int index)
	{
		assert(index >= 0 && index <= 1);
		return (index == 0) ? x : y;
	};

	const T& operator[](int index) const
	{
		assert(index >= 0 && index <= 1);
		return (index == 0) ? x : y;
	};

	bool operator==(const Vec2 & rhs) const { return (x == rhs.x && y == rhs.y); };
	bool operator!=(const Vec2 & rhs) const { return !(*this == rhs); };

	T x{}, y{};
	};