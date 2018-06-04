#include "Vec2.h"



Vec2::Vec2() : x(0.f), y(0.f)
{
}

constexpr Vec2::Vec2(const Vec2 &vec2) = default;

constexpr Vec2::Vec2(float _x, float _y) : x(_x), y(_y)
{
}

// ’P€ +
constexpr Vec2 Vec2::operator +() const
{
	return *this;
}

// ’P€ -
constexpr Vec2 Vec2::operator -() const
{
	return Vec2(-x, -y);
}

// “ñ€ +
constexpr Vec2 Vec2::operator +(Vec2 &other) const
{
	return Vec2(x + other.x, y + other.y);
}

// “ñ€ -
constexpr Vec2 Vec2::operator -(Vec2 &other) const
{
	return Vec2(x - other.x, y - other.y);
}

// 2€ *
constexpr Vec2 Vec2::operator *(float s) const
{
	return Vec2(x * s, y * s);
}

// 2€ /
constexpr Vec2 Vec2::operator /(float s) const
{
	return Vec2(x / s, y / s);
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