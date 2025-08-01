#include "Grid.h"
#include "engine.h"

namespace game1 {

	Grid::Grid() : Grid(5) {}

	Grid::Grid(int size)
		: m_size(size),
		m_tiles(size, std::vector<Tile>(size)),
		m_currentBlock(nullptr),
		m_currentTile(Engine::Vector2i(-1,-1)),
		m_executing(false)
	{
		for (int i = 0; i < m_size; i++) {
			for (int j = 0; j < m_size; j++) {
				m_tiles[i][j] = Grid::Tile(i, j);
			}
		}

		m_emptyTileTex = Engine::Texture2D::Create("assets/textures/grid/empty_tile.png");

		m_tiles[0][0].AttachBlock(Block::GenerateBlock());
	}

	void Grid::Start() {
		if (m_executing) return;

		m_executing = true;
	}
	void Grid::Update() {
		if (!m_executing) return;


	}

	Grid::Tile Grid::GetTile(Engine::Vector2i pos) {
		return m_tiles[pos.x][pos.y];
	}

	void Grid::DrawTiles() {
		for (int i = 0; i < m_size; i++) {
			for (int j = 0; j < m_size; j++) {
				Engine::Renderer2D::DrawQuad(glm::vec3(i * 32, j * 32, 1), { 32, 32 }, m_emptyTileTex);

				Tile* tile = &m_tiles[i][j];
				Block* block = nullptr;
				if (tile->GetBlock(block)) {
					block->Draw(i*32, j*32);
				}
			}
		}
	}
}