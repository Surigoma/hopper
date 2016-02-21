#include <Siv3D.hpp>
#include "world.h"

void Main()
{
	const Font font(30);

	world first_stage;

	while (System::Update())
	{
		first_stage.update();

		for (auto& actor : first_stage.m_actors)
		{
			actor->draw();
		}
	}
}
