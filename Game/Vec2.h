#pragma once
struct Vec2
{
	float x, y;

	constexpr Vec2(float x = 0.f, float y = 0.f);

	float Vec2::length() const;
	constexpr float Vec2::lengthSquare() const;
	constexpr float Vec2::dot(const Vec2& other) const;
	float Vec2::distanceFrom(const Vec2& other) const;
	Vec2 Vec2::normalized() const;
	constexpr bool Vec2::isZero() const;
	constexpr Vec2 operator +() const;
	constexpr Vec2 operator -() const;
	constexpr Vec2 operator +(const Vec2 &other) const;
	constexpr Vec2 operator -(const Vec2 &other) const;
	constexpr Vec2 operator *(float s) const;
	constexpr Vec2 operator /(float s) const;
	Vec2& operator +=(const Vec2& other);
	Vec2& operator -=(const Vec2& other);
	Vec2& operator *=(float s);
	Vec2& operator /=(float s);
};