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

			Tile() : Tile(-1, -1) {}
			Tile(int x, int y) {
				m_gridPos = Engine::Vector2i(x, y);
				m_block = nullptr;
			}
			~Tile() { delete m_block; }
		};

		Grid();
		Grid(int size);
		Tile GetTile(Engine::Vector2 position);

		void DrawTiles();

	private:
		Engine::Ref<Engine::Texture2D> m_emptyTileTex;
		int m_size;
		std::vector<std::vector<Tile>> m_tiles;
	};
}