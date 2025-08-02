#include "Tile.h"
#include "Grid.h"

namespace game1
{
	Tile::Tile() : Tile(-1, -1) {}
	Tile::Tile(int x, int y) {
		m_gridPos = Engine::Vector2i(x, y);
		m_attachedBlock = nullptr;
	}

	bool Tile::GetBlock(Block*& block) {
		if (m_attachedBlock == nullptr) return false;
		block = m_attachedBlock;

		return true;
	}
	void Tile::AttachBlock(Block* block, Grid* grid) {
		if (m_attachedBlock == nullptr) {
			m_attachedBlock = block;
			m_attachedBlock->setPos(grid->TilePosToScreenPos(m_gridPos));
			m_attachedBlock->setTile(this);
		}
	}
	void Tile::DeattachBlock() {
		m_attachedBlock->setTile(nullptr);
		m_attachedBlock = nullptr;
	}
}