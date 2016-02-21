#include <Siv3D.hpp>
#include "world.h"

void Main()
{
	const Font font(30);
	world first_stage;
	Circle c(first_stage.m_actors[0]->m_position, 20.0f);
	while (System::Update())
	{
		first_stage.update();

		for (auto& actor : first_stage.m_actors)
		{
			actor->draw();
		}
		c.setPos(first_stage.m_actors[0]->m_position);
		Vec2 sub = first_stage.m_actors[0]->m_position - Mouse::PosF();
		c.drawArc(Radians(-60.0f)-Atan2(sub.x, sub.y), Radians(120.0f),1.0,1.0,ColorF(100,100,255,200));
		if (first_stage.easing.isActive() || first_stage.easing.isEnd())
		{
			auto bullet_vector = (Mouse::PosF() - first_stage.m_actors[0]->m_position).normalized();
			Circle(bullet_vector * (first_stage.easing.easeOut() + 16.0f) + first_stage.m_actors[0]->m_position, first_stage.easing.easeOut()).draw(Palette::Red);
		}
	}
}
