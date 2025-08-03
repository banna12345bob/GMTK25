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
		m_currentPos(Engine::Vector2i::Invalid()),
		m_executing(false),
		m_currentPoints(0),
		m_msToActivateBlock(3000),
		m_pointsTextDuration(800),
		m_selectedBlock(nullptr),
		m_lastPortalPos(Engine::Vector2i::Invalid()),
		m_teleportNextTurn(false)
	{
		for (int i = 0; i < m_size; i++) {
			for (int j = 0; j < m_size; j++) {
				m_tiles[i][j] = Tile(i, j);
			}
		}

		m_gridOffset = Engine::Vector2i(-m_cellSize * (m_size / 2), -m_cellSize * (m_size / 2) + 40);

		m_emptyTileTex = Engine::Texture2D::Create("assets/textures/grid/empty_tile.png");
		m_outlineTex = Engine::Texture2D::Create("assets/textures/grid/outline.png");
		m_borderTex = Engine::Texture2D::Create("assets/textures/grid/grid_border.png");
		
		Engine::Ref<Engine::Texture2D> font = Engine::Texture2D::Create("assets/textures/fonts/regular_font.png");
		m_textRenderer = new TextRendering(font, { 5, 7 });

		Engine::Ref<Engine::Texture2D> arrowTex = Engine::Texture2D::Create("assets/textures/grid/endpoint_arrows.png");
		for (int i = 0; i < 4; i++) {
			Engine::Ref<Engine::Texture2D> subtex = Engine::SubTexture2D::CreateFromCoords(arrowTex, { i, 0 }, { 16, 16 });
			m_endpointArrowTexs.push_back(subtex);
		}

		m_currentLevel = 1;
		LoadLevel(m_currentLevel);
		m_FontScallingScoring.StartInterpolation(1.f, 1.f, 0.f);
	}

	bool Grid::Start(std::string* errorMessage) {
		// TODO: Check that the "circuit" is valid before going through it

		if (m_executing) {
			if (errorMessage != nullptr) *errorMessage = "Already running!";
			return false;
		}
		else if (!CheckValidCircuit()) {
			if (errorMessage != nullptr) *errorMessage = "Circuit is not valid!";
			return false;
		}

		m_currentPoints = 0;
		m_msActivating = 0;

		m_currentPos = m_startPoint.gridPos;
		m_currentBlock = GetTile(m_currentPos)->GetBlock();

		if (m_currentBlock->getType() == Block::PORTAL) {
			m_lastPortalPos = m_currentPos;
		}
		else {
			m_currentBlock->Activate(&m_currentPoints, &m_blocksActivated, this);
		}
		
		m_executing = true;
		m_msToActivateBlock = 1000;

		Engine::Application::getApplication()->getAudioPlayer()->PlaySound("assets/audio/activate.wav", false, 0.3);

		return true;
	}

	void Grid::Complete() {
		m_executing = false;
		m_currentPos = Engine::Vector2i::Invalid();
		m_currentBlock = nullptr;
		m_pointsText.clear();
		m_blocksActivated.clear();
		m_lastPortalPos = Engine::Vector2i::Invalid();
		EG_TRACE("Circuit complete");

		if (m_currentLevel == 8 && m_currentPoints == m_targetPoints) {
			m_gameLayer->ChangeScene(GameLayer::Scene::Win, false);
			return;
		}
		m_gameLayer->ChangeScene(GameLayer::EndRound, m_currentPoints == m_targetPoints);
	}
	void Grid::NextRound() {
		m_currentLevel++;
		LoadLevel(m_currentLevel);
		m_gameLayer->ChangeScene(GameLayer::Game, true);
	}
	void Grid::Retry() {
		m_currentPoints = 0;
		m_gameLayer->ChangeScene(GameLayer::Game, false);
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

				if (m_currentPos == m_endPoint.gridPos && !m_teleportNextTurn) {
					Complete();
					return;
				}

				Engine::Application::getApplication()->getAudioPlayer()->PlaySound("assets/audio/activate.wav", false, 0.3);

				if (m_teleportNextTurn) {
					Engine::Vector2i temp = m_currentPos;
					m_currentPos = m_lastPortalPos;
					m_currentBlock = GetTile(m_currentPos)->GetBlock();
					m_lastPortalPos = temp;

					m_teleportNextTurn = false;
					m_msActivating = 0;

					Engine::Application::getApplication()->getAudioPlayer()->PlaySound("assets/audio/portal.wav", false, 0.5);

					return;
				}

				Engine::Vector2i nextPos = GetNextBlockPos(m_currentBlock);
				Block* nextBlock = GetTile(nextPos)->GetBlock();

				if (nextBlock->getType() == Block::PORTAL) {
					
					// If we teleported from this portal:
					if (nextPos == m_lastPortalPos) {
						m_currentPos = nextPos;
						m_currentBlock = nextBlock;
					}
					// It is a new portal:
					else {
						// We have nowhere to teleport to (this is the first portal)
						if (m_lastPortalPos == Engine::Vector2i::Invalid()) {
							m_currentPos = nextPos;
							m_currentBlock = nextBlock;
							m_lastPortalPos = m_currentPos;
						}
						// Teleport to the last portal we were at
						else {
							m_teleportNextTurn = true;
							m_currentPos = nextPos;
							m_currentBlock = nextBlock;
						}
					}
				}
				else {
					m_currentPos = nextPos;
					m_currentBlock = nextBlock;
					m_currentBlock->Activate(&m_currentPoints, &m_blocksActivated, this);
				}

				m_msActivating = 0;
			}

			// Check if points text has run out
			/*for (auto it = m_pointsText.begin(); it != m_pointsText.end();) {
				it->msLeft -= deltaTime;
				if (it->msLeft < 0) {
					it = m_pointsText.erase(it);
				}
				else {
					it++;
				}
			}*/
		}
	}

	bool Grid::CheckValidCircuit() {
		Engine::Vector2i currentPos = m_startPoint.gridPos;
		Block* currentBlock = nullptr;
		if (!GetTile(currentPos)->TryGetBlock(currentBlock)) {
			return false;
		}

		// Check if block has 'in' connection where the start point has 'out'
		int side = ((int)m_startPoint.dir + 2) % 4;
		if (currentBlock->getConnection(side) != -1) {
			return false;
		}

		int count = 0;
		while (count < m_size * m_size + 10) { // +10 for good measure. while(true) should work fine, but just in case
			Engine::Vector2i nextPos = GetNextBlockPos(currentBlock);

			if (nextPos.x < 0 || nextPos.x >= m_size || nextPos.y < 0 || nextPos.y >= m_size) {
				return false;
			}

			Block* nextBlock = nullptr;
			if (!GetTile(nextPos)->TryGetBlock(nextBlock)) {
				return false;
			}
			
			if (nextPos == m_endPoint.gridPos) {
				currentPos = nextPos;
				currentBlock = nextBlock;
				break;
			}

			int side = (currentBlock->GetOutDirection() + 2) % 4;
			if (nextBlock->getConnection(side) != -1) {
				return false;
			}

			currentPos = nextPos;
			currentBlock = nextBlock;
		}

		// Endpoint found. Not flipping the direction because only start has it reversed
		if (currentBlock->getConnection((int)m_endPoint.dir) != 1) {
			return false;
		}

		return true;
	}

	Engine::Vector2i Grid::GetNextBlockPos(Block* current) {
		Engine::Vector2i pos = current->getTile()->getPos();

		int outDir = current->GetOutDirection();
		switch (outDir) {
		case 0:
			pos.y += 1;
			break;
		case 1:
			pos.x += 1;
			break;
		case 2:
			pos.y -= 1;
			break;
		case 3:
			pos.x -= 1;
			break;
		}

		return pos;
	}

	void Grid::AddPointsText(int value, Block* block) {
		if (m_pointsText.find(block) == m_pointsText.end()) {
			Engine::Vector2i pos = block->getPos();
			glm::vec3 glmpos = { pos.x, pos.y + 20, 0.93 };
			m_pointsText[block] = PointsText(1, value, glmpos, m_pointsTextDuration);
		}
		else {
			m_pointsText[block].count++;
			m_pointsText[block].points += value;
		}		
	}
	void Grid::AddPointsText(int value, std::vector<Block*>* blocks) {
		for (auto block : *blocks) {
			AddPointsText(value, block);
		}
	}
	int Grid::GetTimesActivated(Block* block) {
		int count = 0;
		for (auto b : m_blocksActivated[block->getType()]) {
			if (b == block) count++;
		}

		return count;
	}

	Block* Grid::GetHoveredBlock(Engine::Vector2i hoveredTile) {
		Block* block = nullptr;
		if (hoveredTile != Engine::Vector2i::Invalid()) {
			if (GetTile(hoveredTile)->TryGetBlock(block)) {
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
	Engine::Vector2i Grid::GridPosToScreenPos(Engine::Vector2i tilePos) {
		return { tilePos.x * m_cellSize + m_gridOffset.x, tilePos.y * m_cellSize + m_gridOffset.y };
	}

	void Grid::RemoveFromUnattachedList(Block* block) {
		auto it = std::find(m_unattachedBlocks.begin(), m_unattachedBlocks.end(), block);
		if (it != m_unattachedBlocks.end()) {
			m_unattachedBlocks.erase(it);
		}
	}

	Grid::Endpoint Grid::CreateEndpoint(Engine::Vector2i gridPos, Endpoint::Type type, Endpoint::Direction dir) {
		Engine::Vector2i tilePos = GridPosToScreenPos(gridPos);

		int offset = (m_cellSize / 2 + 5) * (int)type;

		switch (dir) {
		case Endpoint::UP:
			tilePos.y += offset;
			break;
		case Endpoint::RIGHT:
			tilePos.x += offset;
			break;
		case Endpoint::DOWN:
			tilePos.y -= offset;
			break;
		case Endpoint::LEFT:
			tilePos.x -= offset;
			break;
		}

		return Endpoint(gridPos, { tilePos.x, tilePos.y, 0.9}, type, dir);
	}

	void Grid::Draw() {
		// Border
		Engine::Renderer2D::DrawQuad({ 0, 40, 0.2 }, { 164, 164 }, m_borderTex);

		// Tiles
		for (int i = 0; i < m_size; i++) {
			for (int j = 0; j < m_size; j++) {
				Engine::Vector2i pos = GridPosToScreenPos({ i, j });
				//Engine::Renderer2D::DrawQuad( { pos.x, pos.y, 0.80 }, { m_cellSize, m_cellSize }, m_emptyTileTex);

				Tile* tile = &m_tiles[i][j];
				Block* block = nullptr;
				if (tile->TryGetBlock(block)) {
					Engine::Vector2i vec = Engine::Vector2i(i, j);

					block->Draw(block->getType() == Block::PORTAL && vec != m_lastPortalPos);

					if (m_currentPos == vec) {
						block->PlayScoreAnimation();
					}
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
			m_textRenderer->RenderText(it->second.GetString(), it->first->m_FontScallingScoring.QuadraticEaseOut(), it->second.pos, glm::vec4(1.f), 0.f);
		}

		// Endpoint arrows
		Engine::Renderer2D::DrawQuad(m_startPoint.screenPos, { 16,16 }, m_endpointArrowTexs[m_startPoint.dir]);
		Engine::Renderer2D::DrawQuad(m_endPoint.screenPos, { 16,16 }, m_endpointArrowTexs[m_endPoint.dir]);

		m_textRenderer->RenderText(std::to_string(m_currentPoints), 1.5, {105.f, -90.f, 0.9f}, glm::vec4(1.f), 2.5f);
		m_textRenderer->RenderText(std::to_string(m_targetPoints), 1.5, {105.f, -106.f, 0.9f}, glm::vec4(1.f), 2.5f);
	}
}