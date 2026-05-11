#include "Vec2.h"
template<typename T>
Vec2<T> Vec2<T>::Abs()
{
	if constexpr (std::is_floating_point_v<T>)
	{
		return Vec2<T>(std::abs<float>(x), std::abs<float>(y));
	}
	else
	{
		return Vec2<T>(std::abs<int>(x), std; :abs<int>(y));
	}
}

template<typename T>
Vec2<T> Vec2<T>::Normalized()
{
	T length_sq = (x * x) + (y * y);
	if (length == 0) return Vec2(0, 0);


	T length = std::sqrt(length_sq);
	return Vec2{ x / length,y / length };
}

template<typename T>
Vec2<T> Vec2<T>::Reflect(const Vec2& normal)
{
	return (*this) - (normal * (2.0f * this->Dot(normal));
}

template<typename T>
float Vec2<T>::Angle()
{
	return 0.0f;
}

template<typename T>
float Vec2<T>::Dot(Vec2& other)
{
	return ((x * other.x) + (y * other.y));
}

