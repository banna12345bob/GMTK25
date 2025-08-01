#pragma once

#include "engine.h"
#include "Block.h"
#include <vector>

namespace game1 {

	class Grid
	{
	public:
		struct Tile {
			Engine::Vector2i m_gridPos;
			Block* m_block;
			int m_connections[4] = { 0, 0, 0, 0 };	// Up, left, down, right. 1 for outgoing, -1 for incoming, 0 for none.

			Tile() : Tile(-1, -1) {}
			Tile(int x, int y) {
				m_gridPos = Engine::Vector2i(x, y);
				m_block = nullptr;
			}
			~Tile() { delete m_block; }

			bool GetBlock(Block*& block) {
				if (m_block == nullptr) return false;
				block = m_block;

				return true;
			}
			bool AttachBlock(Block* block) {
				if (m_block != nullptr) return false;
				m_block = block;

				return true;
			}
			
		};

		Grid();
		Grid(int size);
		void Start();
		void Update();
		Tile GetTile(Engine::Vector2i position);

		void DrawTiles();

	private:
		Engine::Ref<Engine::Texture2D> m_emptyTileTex;
		int m_size;
		std::vector<std::vector<Tile>> m_tiles;

		bool m_executing;
		Engine::Vector2i m_currentTile;
		Block* m_currentBlock;
	};
}