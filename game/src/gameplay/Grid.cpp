#include "Grid.h"

namespace game1 {

	Grid::Grid() : Grid(5) {}

	Grid::Grid(int size)
		: m_size(size),
		m_tiles(size, std::vector<Tile>(size)),
		m_currentBlock(nullptr),
		m_currentPos(Engine::Vector2i(-1, -1)),
		m_executing(false),
		m_currentPoints(0),
		m_msToActivateBlock(600)
	{
		for (int i = 0; i < m_size; i++) {
			for (int j = 0; j < m_size; j++) {
				m_tiles[i][j] = Grid::Tile(i, j);
			}
		}

		m_emptyTileTex = Engine::Texture2D::Create("assets/textures/grid/empty_tile.png");
		Engine::Ref<Engine::Texture2D> font = Engine::Texture2D::Create("assets/textures/regular_font.png");
		m_textRenderer = new TextRendering(font, { 5, 7 });

		GetTile(0, 0)->AttachBlock(Block::GenerateBlock());
		GetTile(1, 0)->AttachBlock(Block::GenerateBlock());
		GetTile(2, 0)->AttachBlock(Block::GenerateBlock());
		GetTile(3, 0)->AttachBlock(Block::GenerateBlock());
		GetTile(4, 0)->AttachBlock(new Block(2, Block::ZAIBATSU, Block::HOSAKA, {0, -1,0,1}));

		Start();
	}

	void Grid::Start() {
		// TODO: Check that the "circuit" is valid before going through it

		if (m_executing) return;

		m_currentPoints = 0;
		m_msActivating = 0;

		m_currentPos = Engine::Vector2i(0, 0); // Change this later. Either middle left or top left.
		Block* block = nullptr;
		if (!GetTile(m_currentPos)->GetBlock(block)) {
			return;
		}

		ActivateBlock(block);
		
		m_executing = true;
		m_msToActivateBlock = 600;
	}

	void Grid::Update(int deltaTime) {
		if (!m_executing) return;

		m_msActivating += deltaTime;
		if (m_msActivating > m_msToActivateBlock) {
			int outDir = m_currentBlock->GetOutDirection();
			switch (outDir) {
			case 0: 
				m_currentPos.y += 1;
				break;
			case 1: 
				m_currentPos.x -= 1;
				break;
			case 2: 
				m_currentPos.y -= 1;
				break;
			case 3:
				m_currentPos.x += 1;
				break;
			}

			if (m_currentPos.x >= 5) return; // TEMPORARY

			Block* newBlock = nullptr;
			if (GetTile(m_currentPos)->GetBlock(newBlock)) {
				ActivateBlock(newBlock);
			}
		}

		// Check if points text has run out
		for (auto it = m_pointsText.begin(); it != m_pointsText.end();) {
			it->msLeft -= deltaTime;
			if (it->msLeft < 0) {
				it = m_pointsText.erase(it);
			}
			else {
				it++;
			}
		}
	}

	void Grid::ActivateBlock(Block* block) {
		m_currentBlock = block;
		m_currentBlock->Activate(&m_currentPoints, &m_blocksActivated, m_currentPos, this);

		m_msActivating = 0;

		EG_TRACE("TOTAL POINTS: {0}", m_currentPoints);
	}

	void Grid::AddPointsText(int value, Engine::Vector2i gridPos) {
		glm::vec3 pos = { gridPos.x * m_cellSize, gridPos.y * m_cellSize + 26, 0.85 };
		m_pointsText.push_back(PointsText("+" + std::to_string(value), pos));
	}
	void Grid::AddPointsText(int value, std::vector<Engine::Vector2i>* gridPositions) {
		for (auto gridPos : *gridPositions) {
			AddPointsText(value, gridPos);
		}
	}

	Grid::Tile* Grid::GetTile(Engine::Vector2i pos) {
		return &m_tiles[pos.x][pos.y];
	}
	Grid::Tile* Grid::GetTile(int x, int y) {
		return &m_tiles[x][y];
	}

	void Grid::Draw() {

		// Tiles
		for (int i = 0; i < m_size; i++) {
			for (int j = 0; j < m_size; j++) {
				Engine::Renderer2D::DrawQuad(glm::vec3(i * 32, j * 32, 0.80), { 32, 32 }, m_emptyTileTex);

				Tile* tile = &m_tiles[i][j];
				Block* block = nullptr;
				if (tile->GetBlock(block)) {
					bool activating = m_currentPos.x == i && m_currentPos.y == j;
					block->Draw(i*32, j*32, activating);
				}
			}
		}

		// PointsText
		for (auto it = m_pointsText.begin(); it != m_pointsText.end(); it++) {
			m_textRenderer->RenderText(it->text, 1, it->pos);
		}
	}
}