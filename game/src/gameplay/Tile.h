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

		Block* GetBlock();
		bool TryGetBlock(Block*& block);
		void AttachBlock(Block* block, Grid* grid);
		void DeattachBlock();

		~Tile() { delete m_attachedBlock; }

		Engine::Vector2i getPos() { return m_gridPos; }

	private:
		Engine::Vector2i m_gridPos;
		Block* m_attachedBlock;
	};
}