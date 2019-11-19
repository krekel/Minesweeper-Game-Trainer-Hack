#include <wtypes.h>
#pragma once
namespace game
{
	void displayGrid(std::vector<std::vector<BYTE> >& grid, unsigned int rows, unsigned int cols);
	void fillArray(BYTE* array, std::vector<std::vector<BYTE> >& grid, unsigned int rows, unsigned int cols);
}