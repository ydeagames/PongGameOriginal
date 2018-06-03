#pragma once
struct Vec2
{
	float x, y;

	constexpr Vec2();
	constexpr Vec2(const Vec2 &vec2);
	constexpr Vec2(float x, float y);

	constexpr Vec2 operator +() const;
	constexpr Vec2 operator -() const;
	constexpr Vec2 operator +(Vec2 &other) const;
	constexpr Vec2 operator -(Vec2 &other) const;
	constexpr Vec2 operator *(float s) const;
	constexpr Vec2 operator /(float s) const;
	Vec2& operator +=(const Vec2& other);
	Vec2& operator -=(const Vec2& other);
	Vec2& operator *=(float s);
	Vec2& operator /=(float s);
};