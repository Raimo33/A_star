/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Grid.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: craimond <bomboclat@bidol.juis>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 15:00:10 by craimond          #+#    #+#             */
/*   Updated: 2024/06/14 13:38:02 by craimond         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "headers/Grid.hpp"
#include "headers/Node.hpp"
#include "headers/Tile.hpp"

#include <cmath>

using std::array;

Grid::Grid(const uint16_t n_cols, const uint16_t n_rows, const enum e_cell_type status)
{
	_grid.resize(n_cols);
	for (uint16_t x = 0; x < n_cols; x++)
	{
		_grid[x].resize(n_rows);
		for (uint16_t y = 0; y < n_rows; y++)
			_grid[x][y] = new Node(status, x, y);
	}
}

Grid::~Grid(void)
{
	for (auto &col : _grid)
		for (auto &cell : col)
			delete cell;
}

Cell		&Grid::operator()(const uint16_t x, const uint16_t y) { return *_grid[x][y]; }
const Cell	&Grid::operator()(const uint16_t x, const uint16_t y) const { return *_grid[x][y]; }
uint16_t	Grid::getCols(void) const { return _grid.size(); }
uint16_t	Grid::getRows(void) const { return _grid.empty() ? 0 : _grid[0].size(); }

Cell	&Grid::getStart(void) const
{
	for (auto &col : _grid)
		for (auto &cell : col)
			if (cell->getType() == START)
				return *cell;
	throw StartNotFoundException();
}

Cell	&Grid::getEnd(void) const
{
	for (auto &col : _grid)
		for (auto &cell : col)
			if (cell->getType() == END)
				return *cell;
	throw EndNotFoundException();
}

float	Grid::computeDistance(const Cell &a, const Cell &b)
{
	const Vector2D<uint16_t>	&pos_a = a.getPos();
	const Vector2D<uint16_t>	&pos_b = b.getPos();
	const uint16_t				dx = ::abs(pos_a.x - pos_b.x);
	const uint16_t				dy = ::abs(pos_a.y - pos_b.y);

	return sqrt(dx * dx + dy * dy);
}

void	Grid::reset(void)
{
	for (auto &col : _grid)
		for (auto &cell : col)
			dynamic_cast<Tile *>(cell)->reset();
}

void	Grid::insertCell(const Cell &cell)
{
	const Vector2D<uint16_t>	&pos = cell.getPos();

	delete _grid[pos.x][pos.y];
	_grid[pos.x][pos.y] = new Node(cell);
}

const char *Grid::StartNotFoundException::what(void) const throw() { return "Start not found"; }
const char *Grid::EndNotFoundException::what(void) const throw() { return "End not found"; }