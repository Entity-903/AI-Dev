#include "CAScene.h"
#include <random>

color_t white{ 255, 255, 255, 255 };
color_t black{ 0, 0, 0, 255 };

bool CAScene::Initialize()
{
	m_renderer.Initialize();
	m_renderer.CreateWindow("CAScene", 800, 600);

	m_input.Initialize();
	m_input.Update();

	m_framebuffer = std::make_unique<Framebuffer>(m_renderer, m_renderer.m_width, m_renderer.m_height);
	m_cells = std::make_unique<Cells<bool>>(m_renderer.m_width, m_renderer.m_height);
	//m_cells = std::make_unique<Cells<uint8_t>>(m_renderer.m_width, m_renderer.m_height);

	return true;
}

void CAScene::Update()
{
	Scene::Update();

	m_cells->Write(m_cells->m_width / 2, 0, 1);

	int rule = 18;
	for (int y = 0; y < m_cells->m_height - 1; y++)
	{
		for (int x = 1; x < m_cells->m_width - 1; x++)
		{
			int i = 0; // variable to store the 3-bit neighborhood pattern (0-7) for the current cell.

			// create the 3-bit neighborhood pattern from the values of the neighboring cells in the current row.
			i |= (int)(m_cells->Read(x - 1, y) << 2);	// [X,0,0]
			i |= (int)(m_cells->Read(x + 0, y) << 1);	// [0,X,0]
			i |= (int)(m_cells->Read(x + 1, y) << 0);	// [0,0,X]

			// extract the new state of the cell based on the rule:
			// - the `rule` is a number where each bit represents the output state for a specific 3-bit input pattern.
			// - use a bitwise AND to isolate the bit corresponding to the current neighborhood pattern `i`.
			uint8_t state = (rule & (1 << i)) ? 1 : 0;

			// write the calculated state to the cell directly below the current cell (next generation).
			m_cells->Write(x, y + 1, state);
		}
	}

	m_framebuffer->Clear(color_t{ 0, 0, 0, 255 });
	// write cells to the framebuffer
	for (int i = 0; i < m_cells->m_data.size(); i++)
	{
		if (m_cells->m_data[i])
		{
			m_framebuffer->m_buffer[i] = color_t{ 255, 255, 255, 255 };
		}
	}
}

void CAScene::Draw()
{

	// update framebuffer
	m_framebuffer->Update();

	// show screen
	m_renderer = *m_framebuffer;
	SDL_RenderPresent(m_renderer.m_renderer);
}
