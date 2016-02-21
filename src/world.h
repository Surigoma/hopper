#pragma once

#include <vector>
#include "actor.h"

class world
{
public:
	world()
		:easing(0.0f, 100.0f, Easing::Quint, 10000)
	{
		m_actors.emplace_back(std::make_shared<player>(L"player", Vec2(60.0, 10.0)));
	}

	~world() = default;
	EasingController<float> easing;
	enum ShotType : int{
		Chage = 0,
		Rapid,
		End
	} shot;
	void update()
	{
		const double delta_time = 1.0 / 60.0;
		const double speed = 800.0;

		if (Input::KeyA.pressed)
		{
			m_actors[0]->impulse({ -speed, 0.0 });
		}
		if (Input::KeyD.pressed)
		{
			m_actors[0]->impulse({ speed, 0.0 });
		}

		if (Input::KeyW.pressed)
		{
			m_actors[0]->impulse({ 0.0, -speed });
		}
		if (Input::KeySpace.clicked)
		{
			shot = static_cast<ShotType>(shot+1);
			if (shot == ShotType::End) { shot = static_cast<ShotType>(0); }
			easing.reset();
		}

		if (Input::KeyS.pressed)
		{
			m_actors[0]->impulse({ 0.0, speed });
		}
		if (Input::MouseL.pressed)
		{
			switch (shot)
			{
			case ShotType::Chage:
				if (!easing.isActive() && Input::MouseL.pressedDuration == 0)
					easing.start();
				break;
			case ShotType::Rapid:
				if (System::FrameCount() % 3 == 0)
				{
					auto bullet_vector = (Mouse::PosF() - m_actors[0]->m_position).normalized();
					m_actors.emplace_back(std::make_shared<actor>(L"bullet", m_actors[0]->m_position + bullet_vector * 16.0f, bullet_vector * 500.0, 3.0f));
				}
			default:
				break;
			}
		}
		if (Input::MouseL.released)
		{
			switch (shot)
			{
			case ShotType::Chage:
				if (easing.easeOut() > 5)
				{
					auto bullet_vector = (Mouse::PosF() - m_actors[0]->m_position).normalized();
					m_actors.emplace_back(std::make_shared<actor>(L"bullet", m_actors[0]->m_position + bullet_vector * (easing.easeOut() + 16.0f), bullet_vector * 500.0, easing.easeOut()));
				}
				easing.reset();
				break;
			default:
				break;
			}
		}

		if (System::FrameCount() % 50 == 0 && RandomBool(0.5))
		{
			m_actors.emplace_back(std::make_shared<enemy>(L"enemy", RandomVec2(640,480), RandomVec2(500, 500)));
		}

		for (auto& actor : m_actors)
		{
			actor->update(delta_time);
		}

		for (auto i = m_actors.begin(); i != m_actors.end(); i++)
		{
			for (auto j = i + 1; j != m_actors.end(); j++)
			{
				if ((*i)->get_bounding_object().intersects((*j)->get_bounding_object()) && (*i != *j))
				{
					(*i)->collide_event();
					(*j)->collide_event();
				}
			}
			solve(**i);
		}

		Erase_if(m_actors, [](const std::shared_ptr<actor>& _actor) {return  _actor->m_has_killed; });
	}

	void solve(actor& _actor)
	{
		if (_actor.m_can_reflect)
		{
			if (_actor.m_position.y < 0 && _actor.m_velocity.y < 0.0)
			{
				_actor.m_velocity.y *= -1.0;
			}

			if (_actor.m_position.x < 0 && _actor.m_velocity.x < 0.0)
			{
				_actor.m_velocity.x *= -1.0;
			}

			if (_actor.m_position.y > Window::Height() && _actor.m_velocity.y > 0.0)
			{
				_actor.m_velocity.y *= -1.0;
			}

			if (_actor.m_position.x > Window::Width() && _actor.m_velocity.x > 0.0)
			{
				_actor.m_velocity.x *= -1.0;
			}
		}
	}

public:
	Array<std::shared_ptr<actor>> m_actors;
};
