#pragma once

#include <Siv3D.hpp>

class actor
{
public:
	actor(const String& _tag, const Vec2& _position = Vec2::Zero, const Vec2& _velocity = Vec2::Zero, double _radius = 5.0)
		: m_tag(_tag), m_position(_position), m_velocity(_velocity), m_acceleration(Vec2::Zero), m_radius(_radius), m_can_reflect(true), m_has_killed(false)
	{
	}

	virtual ~actor() = default;

	virtual void update(double _delta_time)
	{
		m_velocity += m_acceleration * _delta_time;
		m_position += m_velocity * _delta_time;
		m_acceleration = Vec2::Zero;
	}

	virtual void draw() const
	{
		get_bounding_object().draw(Palette::Red);
	}

	void impulse(const Vec2& _acceleration)
	{
		m_acceleration += _acceleration;
	}

	Circle get_bounding_object() const
	{
		return Circle(m_position, m_radius);
	}

	virtual void collide_event()
	{
		m_has_killed = true;
	}

public:
	String m_tag;
	Vec2 m_position;
	Vec2 m_velocity;
	Vec2 m_acceleration;
	double m_radius;
	bool m_can_reflect;
	bool m_has_killed;
};

class player : public actor
{
public:
	player(const String& _tag, const Vec2& _position = Vec2::Zero, const Vec2& _velocity = Vec2::Zero)
		: actor(_tag, _position, _velocity, 15.0)
	{
	}
	
	virtual ~player() = default;

	void draw() const
	{
		get_bounding_object().draw(Palette::White);
	}

	void collide_event()
	{
	}
};

class enemy : public actor
{
public:
	enemy(const String& _tag, const Vec2& _position = Vec2::Zero, const Vec2& _velocity = Vec2::Zero)
		: actor(_tag, _position, _velocity, 15.0)
	{
	}

	virtual ~enemy() = default;

	void draw() const
	{
		get_bounding_object().draw(Palette::Green);
	}

	void collide_event()
	{
		m_has_killed = true;
	}
};