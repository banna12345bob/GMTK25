#include "Grid.h"
#include "engine.h"

namespace game1 {

	Grid::Grid() : Grid(16) {}

	Grid::Grid(int size) : m_size(size), m_tiles(size, std::vector<Tile>(size)) {
		for (int i = 0; i < m_size; i++) {
			for (int j = 0; j < m_size; j++) {
				m_tiles[i][j] = Grid::Tile(i, j);
			}
		}

		m_emptyTileTex = Engine::Texture2D::Create("assets/textures/tile.png");
	}

	Grid::Tile Grid::GetTile(Engine::Vector2 pos) {
		return m_tiles[pos.x][pos.y];
	}

	void Grid::DrawTiles() {
		for (int i = 0; i < m_size; i++) {
			for (int j = 0; j < m_size; j++) {
				Engine::Renderer2D::DrawQuad(glm::vec3(i * 1, j * 1, 0), { 1, 1 }, m_emptyTileTex);
			}
		}
	}
}