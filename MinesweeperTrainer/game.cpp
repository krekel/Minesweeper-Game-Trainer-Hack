#include "stdafx.h"
#include "game.h"
#include <iostream>
#include <vector>

void game::displayGrid(std::vector<std::vector<BYTE> >& grid, unsigned int rows, unsigned int cols)
{
	for (size_t i = 0; i < rows; i++)
	{
		for (size_t j = 0; j < cols; j++)
		{
			BYTE val = grid[i][j];

			if (val == 0x10)
				std::cout << "[#]";
			else if (val == 0x8f)
				std::cout << "[*]";
			else
			{
				if ((int)val == 0)
				{
					std::cout << "[ ]";
				}
				else
				{
					std::cout << "[" << (int)val << "]";
				}
			}
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void game::fillArray(BYTE * array, std::vector<std::vector<BYTE> > & grid, unsigned int rows, unsigned int cols)
{
	for (size_t i = 0; i < 27; i++)
	{
		BYTE* ptr = array + (32 * i);
		for (size_t j = 0; j < 32; j++)
		{
			BYTE val = *(ptr + j);
			grid[i][j] = val;
		}

	}

	int mines = 0;
	for (size_t i = 1; i < rows; i++)
	{
		for (size_t j = 1; j < cols; j++)
		{
			BYTE val = grid[i][j];

			if (val != 0x10 && val != 0x8F)
			{
				//Top
				if (grid[i - 1][j] == 0x8F)
					mines++;
				//Top Right
				if (grid[i - 1][j + 1] == 0x8F)
					mines++;
				//Top Left
				if (grid[i - 1][j - 1] == 0x8F)
					mines++;
				//Left
				if (grid[i][j - 1] == 0x8F)
					mines++;
				//Right
				if (grid[i][j + 1] == 0x8F)
					mines++;
				//Bottom
				if (grid[i + 1][j] == 0x8F)
					mines++;
				//Bottom Right
				if (grid[i + 1][j + 1] == 0x8F)
					mines++;
				//Bottom Left
				if (grid[i + 1][j - 1] == 0x8F)
					mines++;
			}
			else
			{
				continue;
			}

			grid[i][j] = mines;
			mines = 0;
		}
	}


}