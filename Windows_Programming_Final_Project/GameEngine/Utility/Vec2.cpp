#include "pch.h"
#include "Vec2.h"

template<typename T>
float Vec2<T>::LengthSq()
{
	return x * x + y * y;
}
template<typename T>
float Vec2<T>::Length()
{
	return std::sqrt(LengthSq());
}
template<typename T>
Vec2<T> Vec2<T>::Abs()
{
	if constexpr (std::is_floating_point_v<T>)
	{
		return Vec2<T>(std::abs(x), std::abs(y));
	}
	else
	{
		return Vec2<T>(std::abs(x), std::abs(y));
	}
}

template<typename T>
Vec2<T> Vec2<T>::Normalized()
{
	auto length_sq = LengthSq();
	if (length_sq < 0.00000001f)
		return Vec2<T>{0, 0};


	T length = Length();
	return Vec2<T>{ static_cast<T>(x / length), static_cast<T>(y / length) };
}

template<typename T>
Vec2<T> Vec2<T>::Reflect(Vec2& other)
{
	return (*this) - (other * (2.0f * this->Dot(other)));
}

template<typename T>
float Vec2<T>::Angle()
{
	return std::atan2((float)y, (float)x);
}

template<typename T>
float Vec2<T>::Dot(Vec2& other)
{
	return ((x * other.x) + (y * other.y));
}

template<typename T>
float Vec2<T>::Cross(Vec2& other)
{
	return x * other.y - y * other.x;
}

template<typename T>
bool Vec2<T>::Is_Equal_Approx(Vec2& other)
{
	const float EP{ 0.00001f };
	
	return (std::abs(x - other.x) < EP && std::abs(y - other.y) < EP);
}

template<typename T>
bool Vec2<T>::is_Normalized()
{
	return (x + y == 1.0f);
}

template class Vec2<float>;
template class Vec2<int>;
