#include "Vec2.h"
#include <cmath>


constexpr Vec2::Vec2(float x, float y) : x(x), y(y)
{
}

// ’·‚³
float Vec2::length() const
{
	return std::sqrt(lengthSquare());
}

// ’·‚³‚Ì“ñæ
constexpr float Vec2::lengthSquare() const
{
	return dot(*this);
}

// “àÏ
constexpr float Vec2::dot(const Vec2& other) const
{
	return x * other.x + y * other.y;
}

// “ñ“_ŠÔ‹——£
float Vec2::distanceFrom(const Vec2& other) const
{
	return (other - *this).length();
}

// ³‹K‰»
Vec2 Vec2::normalized() const
{
	return *this / length();
}

// 0ƒxƒNƒgƒ‹
constexpr bool Vec2::isZero() const
{
	return x == 0.0 && y == 0.0;
}

// ’P€ +
constexpr Vec2 Vec2::operator +() const
{
	return *this;
}

// ’P€ -
constexpr Vec2 Vec2::operator -() const
{
	return {-x, -y};
}

// “ñ€ +
constexpr Vec2 Vec2::operator +(const Vec2 &other) const
{
	return {x + other.x, y + other.y};
}

// “ñ€ -
constexpr Vec2 Vec2::operator -(const Vec2 &other) const
{
	return {x - other.x, y - other.y};
}

// 2€ *
constexpr Vec2 Vec2::operator *(float s) const
{
	return {x * s, y * s};
}

// 2€ /
constexpr Vec2 Vec2::operator /(float s) const
{
	return {x / s, y / s};
}

// •¡‡‘ã“ü‰‰Z +=
Vec2& Vec2::operator +=(const Vec2& other)
{
	x += other.x;
	y += other.y;
	return *this;
}

// •¡‡‘ã“ü‰‰Z -=
Vec2& Vec2::operator -=(const Vec2& other)
{
	x -= other.x;
	y -= other.y;
	return *this;
}

// •¡‡‘ã“ü‰‰Z *=
Vec2& Vec2::operator *=(float s)
{
	x *= s;
	y *= s;
	return *this;
}

// •¡‡‘ã“ü‰‰Z /=
Vec2& Vec2::operator /=(float s)
{
	x /= s;
	y /= s;
	return *this;
}