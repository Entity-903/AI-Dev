#include "GOLScene.h"
#include <random>

color_t white{ 255, 255, 255, 255 };
color_t black{ 0, 0, 0, 255 };

bool GOLScene::Initialize()
{
	m_renderer.Initialize();
	m_renderer.CreateWindow("Game of Life", 800, 600);

	m_input.Initialize();
	m_input.Update();

	m_framebuffer = std::make_unique<Framebuffer>(m_renderer, m_renderer.m_width, m_renderer.m_height);
	m_cellsA = std::make_unique<Cells<uint8_t>>(m_renderer.m_width, m_renderer.m_height);
	m_cellsB = std::make_unique<Cells<uint8_t>>(m_renderer.m_width, m_renderer.m_height);

	return true;
}

void GOLScene::Update()
{
	Scene::Update();
	m_frame++;

	Cells<uint8_t>* currentCells = (m_frame % 2) ? m_cellsA.get() : m_cellsB.get();
	Cells<uint8_t>* nextCells = (m_frame % 2) ? m_cellsB.get() : m_cellsA.get();

	std::fill(nextCells->m_data.begin(), nextCells->m_data.end(), 0);

	//
	//if (m_input.GetKeyDown(SDL_SCANCODE_SPACE))
	//{
	//	for ()
	//	{
	//
	//	}
	//}
	//

	for (int y = 1; y < nextCells->m_height - 1; y++)
	{
		for (int x = 1; x < nextCells->m_width - 1; x++)
		{
			int count = 0;

			// Make this a for() loop that excludes checking the center location (0, 0)

			for (int a = -1; a < 2; a++)
			{
				for (int b = -1; b < 2; b++)
				{
					if (!(a == 0 && b == 0))
					{
						count += currentCells->Read(x + a, y + b);
					}
				}
			}

			//-----------------------------
			//count += currentCells->Read(x - 1, y - 1);
			//count += currentCells->Read(x,     y - 1 );
			//count += currentCells->Read(x + 1, y - 1);
			//
			//count += currentCells->Read(x - 1, y);
			//count += currentCells->Read(x + 1, y);
			//
			//count += currentCells->Read(x - 1, y + 1);
			//count += currentCells->Read(x,     y + 1 );
			//count += currentCells->Read(x + 1, y + 1);

			// do the game of life rules
			uint8_t currentState = currentCells->Read(x, y);
			uint8_t nextState;
			if (currentState)
			{
				// alive, stay alive if 2-3 neighbors, else die
				if (!(count == 2 || count == 3))
				{
					nextState = 0;
				}
				//nextState = (count == 2 || count == 3) ? 1 : 0;
			}
			else
			{
				// dead, make alive if 3 neighbors
				if (count == 3)
				{
					nextState = 1;
				}
				//nextState = (count == 3) ? 1 : 0;
			}

			nextCells->Write(x, y, nextState);
		}
	}

	//m_cells->Write(m_cells->m_width / 2, 0, 1);

	//int rule = 18;
	//for (int y = 0; y < m_cells->m_height - 1; y++)
	//{
	//	for (int x = 1; x < m_cells->m_width - 1; x++)
	//	{
	//		int i = 0; // variable to store the 3-bit neighborhood pattern (0-7) for the current cell.
	//
	//		// create the 3-bit neighborhood pattern from the values of the neighboring cells in the current row.
	//		i |= (int)(m_cells->Read(x - 1, y) << 2);	// [X,0,0]
	//		i |= (int)(m_cells->Read(x + 0, y) << 1);	// [0,X,0]
	//		i |= (int)(m_cells->Read(x + 1, y) << 0);	// [0,0,X]
	//
	//		// extract the new state of the cell based on the rule:
	//		// - the `rule` is a number where each bit represents the output state for a specific 3-bit input pattern.
	//		// - use a bitwise AND to isolate the bit corresponding to the current neighborhood pattern `i`.
	//		uint8_t state = (rule & (1 << i)) ? 1 : 0;
	//
	//		// write the calculated state to the cell directly below the current cell (next generation).
	//		m_cells->Write(x, y + 1, state);
	//	}
	//}

	m_framebuffer->Clear(color_t{ 0, 0, 0, 255 });
	// write cells to the framebuffer
	//for (int i = 0; i < m_cells->m_data.size(); i++)
	//{
	//	if (m_cells->m_data[i])
	//	{
	//		m_framebuffer->m_buffer[i] = color_t{ 255, 255, 255, 255 };
	//	}
	//}
}

void GOLScene::Draw()
{

	// update framebuffer
	m_framebuffer->Update();

	// show screen
	m_renderer = *m_framebuffer;
	SDL_RenderPresent(m_renderer.m_renderer);
}
