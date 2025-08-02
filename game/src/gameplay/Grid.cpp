#include "Grid.h"
#include "Tile.h"
#include "engine.h"
#include "GameLayer.h"

namespace game1 {

	Grid::Grid(int size, GameLayer* gameLayer)
		: m_size(size),
		m_gameLayer(gameLayer),
		m_tiles(size, std::vector<Tile>(size)),
		m_currentBlock(nullptr),
		m_currentPos(Engine::Vector2i(-1, -1)),
		m_executing(false),
		m_currentPoints(0),
		m_msToActivateBlock(1000),
		m_pointsTextDuration(800),
		m_selectedBlock(nullptr)
	{
		for (int i = 0; i < m_size; i++) {
			for (int j = 0; j < m_size; j++) {
				m_tiles[i][j] = Tile(i, j);
			}
		}

		m_gridOffset = Engine::Vector2i(-m_cellSize * (m_size / 2), -m_cellSize * (m_size / 2) + 40);

		m_emptyTileTex = Engine::Texture2D::Create("assets/textures/grid/empty_tile.png");
		m_outlineTex = Engine::Texture2D::Create("assets/textures/grid/outline.png");
		Engine::Ref<Engine::Texture2D> font = Engine::Texture2D::Create("assets/textures/fonts/regular_font.png");
		m_textRenderer = new TextRendering(font, { 5, 7 });

		Engine::Vector2i tilePos = { 2, 2 };
		GetTile(tilePos)->AttachBlock(new Block(TilePosToScreenPos(tilePos), GetTile(tilePos), 4, Block::HOSAKA, Block::NONE, { 0, -1, 1, 0}), this);
		
		tilePos = { 2, 3 };
		GetTile(tilePos)->AttachBlock(new Block(TilePosToScreenPos(tilePos), GetTile(tilePos), 4, Block::MAAS_BIOLABS, Block::NONE, { 0, -1, 1, 0}), this);

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
		m_msToActivateBlock = 1000;
	}

	void Grid::Update(Engine::Timestep deltaTime) {
		if (!m_executing) {
			// Check for player input
			if (Engine::Mouse::wasButtonPressed(EG_MOUSECODE_LEFT)) {
				
				Engine::Vector2i hoveredTile = GetHoveredTile();
				Block* selectedBlock = GetHoveredBlock(hoveredTile);
				
				// Select the block
				if (m_selectedBlock == nullptr) {
					m_selectedBlock = selectedBlock;
					return;
				}

				// De-select
				if (selectedBlock == m_selectedBlock) {
					m_selectedBlock = nullptr;
					return;
				}

				// Swap blocks
				if (selectedBlock != nullptr) {
					SwapBlocks(m_selectedBlock, selectedBlock);
					m_selectedBlock = nullptr;
					return;
				}

				// Move either to grid space or outside of the grid
				MoveBlock(m_selectedBlock, hoveredTile);
				m_selectedBlock = nullptr;
			}
			else if (Engine::Mouse::wasButtonPressed(EG_MOUSECODE_RIGHT)) {
				Engine::Vector2i hoveredTile = GetHoveredTile();
				Block* hoveredBlock = GetHoveredBlock(hoveredTile);
				if (hoveredBlock != nullptr) {
					hoveredBlock->Rotate();
				}
			}
		}
		else {

			// Run execution logic
			m_msActivating += deltaTime;
			if (m_msActivating > m_msToActivateBlock) {
				int outDir = m_currentBlock->GetOutDirection();
				switch (outDir) {
				case 0:
					m_currentPos.y += 1;
					break;
				case 1:
					m_currentPos.x += 1;
					break;
				case 2:
					m_currentPos.y -= 1;
					break;
				case 3:
					m_currentPos.x -= 1;
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
	}

	void Grid::ActivateBlock(Block* block) {
		m_currentBlock = block;
		m_currentBlock->Activate(&m_currentPoints, &m_blocksActivated, this);

		m_msActivating = 0;

		EG_TRACE("TOTAL POINTS: {0}", m_currentPoints);
	}

	void Grid::AddPointsText(int value, Block* block) {
		Engine::Vector2i pos = block->getPos();
		glm::vec3 glmpos = { pos.x * m_cellSize + m_gridOffset.x, pos.y * m_cellSize + m_gridOffset.y + 26, 0.85 };
		m_pointsText.push_back(PointsText("+" + std::to_string(value), glmpos, m_pointsTextDuration));
	}
	void Grid::AddPointsText(int value, std::vector<Block*>* blocks) {
		for (auto block : *blocks) {
			AddPointsText(value, block);
		}
	}

	Block* Grid::GetHoveredBlock(Engine::Vector2i hoveredTile) {
		Block* block = nullptr;
		if (hoveredTile != Engine::Vector2i::Invalid()) {
			if (GetTile(hoveredTile)->GetBlock(block)) {
				return block;
			}
		}
		else {
			glm::vec2 mouseGamePos = m_gameLayer->GetMouseGamePosition();

			for (auto block : m_unattachedBlocks) {
				if (block->Hovering(Engine::Vector2i(mouseGamePos.x, mouseGamePos.y))) {
					return block;
				}
			}
		}

		return nullptr;
	}
	void Grid::SwapBlocks(Block* b1, Block* b2) {
		Tile* t1 = b1->getTile();
		Tile* t2 = b2->getTile();


		if (t1 == nullptr) {
			if (t2 == nullptr) {
				Engine::Vector2i tempPos = b1->getPos();
				b1->setPos(b2->getPos());
				b2->setPos(tempPos);
			}
			else {
				t2->DeattachBlock();
				b2->setPos(b1->getPos());
				t2->AttachBlock(b1, this);

				RemoveFromUnattachedList(b1);
				m_unattachedBlocks.push_back(b2);
			}
		}
		else if (t2 == nullptr) {
			if (t1 == nullptr) {
				Engine::Vector2i tempPos = b1->getPos();
				b1->setPos(b2->getPos());
				b2->setPos(tempPos);
			}
			else {
				t1->DeattachBlock();
				b1->setPos(b2->getPos());
				t1->AttachBlock(b2, this);

				m_unattachedBlocks.push_back(b1);
				RemoveFromUnattachedList(b2);
			}
		}
		else {
			t1->DeattachBlock();
			t2->DeattachBlock();

			t1->AttachBlock(b2, this);
			t2->AttachBlock(b1, this);
		}
	}
	void Grid::MoveBlock(Block* block, Engine::Vector2i hoveredTile) {
		if (block->getTile() == nullptr) {
			RemoveFromUnattachedList(block);
		}
		else {
			block->getTile()->DeattachBlock();
		}
		
		if (hoveredTile != Engine::Vector2i::Invalid()) {
			GetTile(hoveredTile)->AttachBlock(block, this);
		}
		else {
			Engine::Vector2i mousePos = m_gameLayer->GetMouseGamePosition();
			block->setPos(mousePos);
			m_unattachedBlocks.push_back(block);
		}
	}
	Engine::Vector2i Grid::GetHoveredTile() {
		if (!HoveringGrid()) {
			return Engine::Vector2i::Invalid();
		}

		// Duplicated code but I can fix it later
		// Also, the position is 0 when it should be -1 (still 0 when meant to be 0 though), I think something to do with how the mouse position is calculated
		glm::vec2 mouseGamePos = m_gameLayer->GetMouseGamePosition();
		Engine::Vector2i tilePos = Engine::Vector2i(mouseGamePos.x - m_gridOffset.x + m_cellSize / 2, mouseGamePos.y - m_gridOffset.y + m_cellSize / 2);
		tilePos /= m_cellSize;

		return tilePos;
	}

	bool Grid::HoveringGrid() {
		glm::vec2 mouseGamePos = m_gameLayer->GetMouseGamePosition();
		Engine::Vector2i pos = Engine::Vector2i(mouseGamePos.x - m_gridOffset.x + m_cellSize / 2, mouseGamePos.y - m_gridOffset.y + m_cellSize / 2);

		return pos.x >= 0 && pos.x < m_size * m_cellSize && pos.y >= 0 && pos.y < m_size * m_cellSize;
	}

	Tile* Grid::GetTile(Engine::Vector2i pos) {
		return &m_tiles[pos.x][pos.y];
	}
	Tile* Grid::GetTile(int x, int y) {
		return &m_tiles[x][y];
	}
	Engine::Vector2i Grid::TilePosToScreenPos(Engine::Vector2i tilePos) {
		return { tilePos.x * m_cellSize + m_gridOffset.x, tilePos.y * m_cellSize + m_gridOffset.y };
	}

	void Grid::RemoveFromUnattachedList(Block* block) {
		auto it = std::find(m_unattachedBlocks.begin(), m_unattachedBlocks.end(), block);
		if (it != m_unattachedBlocks.end()) {
			m_unattachedBlocks.erase(it);
		}
	}

	void Grid::Draw() {
		// Tiles
		for (int i = 0; i < m_size; i++) {
			for (int j = 0; j < m_size; j++) {
				Engine::Vector2i pos = TilePosToScreenPos({ i, j });
				Engine::Renderer2D::DrawQuad(glm::vec3(pos.x, pos.y, 0.80), { m_cellSize, m_cellSize }, m_emptyTileTex);

				Tile* tile = &m_tiles[i][j];
				Block* block = nullptr;
				if (tile->GetBlock(block)) {
					bool activating = m_currentPos == Engine::Vector2i(i, j);
					block->Draw(activating);
				}
			}
		}

		// Tiles outside of the grid
		for (auto it = m_unattachedBlocks.begin(); it != m_unattachedBlocks.end(); it++) {
			(*it)->Draw(false);
		}

		// Tile Outline
		if (m_selectedBlock != nullptr) {
			m_selectedBlock->DrawOutline();
		}

		// PointsText
		for (auto it = m_pointsText.begin(); it != m_pointsText.end(); it++) {
			m_textRenderer->RenderText(it->text, 1, it->pos);
		}

		m_textRenderer->RenderText(std::to_string(m_currentPoints), 1.5, {105.f, -100.f, 0.9f}, glm::vec4(1.f), 2.5f);
	}
}