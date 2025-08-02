#pragma once

#include "engine.h"
#include "Block.h"


namespace game1
{
	class Grid;

	struct Tile
	{
	public:
		Tile();
		Tile(int x, int y);

		bool GetBlock(Block*& block);
		void AttachBlock(Block* block, Grid* grid);
		void DeattachBlock();

	private:
		Engine::Vector2i m_gridPos;
		Block* m_attachedBlock;
	};
}