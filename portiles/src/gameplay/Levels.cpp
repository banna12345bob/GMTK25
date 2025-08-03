#pragma once

#include "Grid.h"
#include <random>

namespace game1 {

	void Grid::LoadLevel(int level) {
		m_currentPoints = 0;
		m_unattachedBlocks.clear();
		m_tiles = std::vector<std::vector<Tile>>(m_size, std::vector<Tile>(m_size));
		for (int i = 0; i < m_size; i++) {
			for (int j = 0; j < m_size; j++) {
				m_tiles[i][j] = Tile(i, j);
			}
		}

		switch (level) {
		case 1:
			Level1();
			break;
		case 2:
			Level2();
			break;
		case 3:
			Level3();
			break;
		case 4:
			Level4();
			break;
		case 5:
			Level5();
			break;
		case 6:
			Level6();
			break;
		case 7:
			Level7();
			break;
		case 8:
			Level8();
			break;

		default:
			break;
		}
	}

	void Grid::PlaceBlocks(std::vector<Block*> blocks) {
		// Spawn half outside of the grid
		int halfwidth = (blocks.size() / 2 * 36) / 2;
		for (int i = 0; i < blocks.size() / 2; i++) {
			Engine::Vector2i pos = { -halfwidth + 36 * i, -64 };
			blocks[i]->setPos(pos);

			for (int j = std::rand() % 4; j >= 0; j--) {
				blocks[i]->Rotate();
			}

			m_unattachedBlocks.push_back(blocks[i]);
		}

		// Spawn other half on grid
		for (int i = blocks.size() / 2; i < blocks.size(); i++) {
			while (true) {
				Engine::Vector2i gridPos = { std::rand() % m_size, std::rand() % m_size };

				Tile* tile = GetTile(gridPos);
				if (!tile->GetBlock()) {
					tile->AttachBlock(blocks[i], this);
					break;
				}
			}

			for (int j = std::rand() % 4; j >= 0; j--) {
				blocks[i]->Rotate();
			}
		}
	}

	void Grid::Level1() {

		Engine::Vector2i vec = Engine::Vector2i::Invalid();
		std::vector<Block*> blocks;

		blocks.push_back(new Block(vec, nullptr, 1, Block::HOSAKA, Block::NONE, { 0, 1 , 0, -1 }));
		blocks.push_back(new Block(vec, nullptr, 2, Block::HOSAKA, Block::NONE, { 0, 1 , 0, -1 }));
		blocks.push_back(new Block(vec, nullptr, 1, Block::HOSAKA, Block::NONE, { 0, 0 , 1, -1 }));
		blocks.push_back(new Block(vec, nullptr, 1, Block::HOSAKA, Block::NONE, { 0, 1 , 0, -1 }));
		blocks.push_back(new Block(vec, nullptr, 1, Block::HOSAKA, Block::NONE, { 0, 1 , 0, -1 }));

		blocks.push_back(new Block(vec, nullptr, 1, Block::HOSAKA, Block::NONE, { 0, 1 , 0, -1 }));
		blocks.push_back(new Block(vec, nullptr, 2, Block::HOSAKA, Block::NONE, { 0, 1 , -1, 0 }));
		PlaceBlocks(blocks);

		m_startPoint = CreateEndpoint({ 0, 2 }, Endpoint::START, Endpoint::RIGHT);
		m_endPoint = CreateEndpoint({ 2, 0 }, Endpoint::END, Endpoint::DOWN);

		m_targetPoints = 6;
	}

	void Grid::Level2() {

		Engine::Vector2i vec = Engine::Vector2i::Invalid();
		std::vector<Block*> blocks;

		blocks.push_back(new Block(vec, nullptr, 1, Block::HOSAKA, Block::NONE, { 0, 1 , 0, -1 }));
		blocks.push_back(new Block(vec, nullptr, 1, Block::HOSAKA, Block::NONE, { 0, 1 , 0, -1 }));
		blocks.push_back(new Block(vec, nullptr, 2, Block::ZAIBATSU, Block::HOSAKA, { -1, 0 , 1, 0 }));
		blocks.push_back(new Block(vec, nullptr, 1, Block::MAAS_BIOLABS, Block::NONE, { 0, 1 , 0, -1 }));
		blocks.push_back(new Block(vec, nullptr, 1, Block::MAAS_BIOLABS, Block::NONE, { 0, 0 , 1, -1 }));
		PlaceBlocks(blocks);

		m_startPoint = CreateEndpoint({ 0, 0 }, Endpoint::START, Endpoint::RIGHT);
		m_endPoint = CreateEndpoint({ 3, 0 }, Endpoint::END, Endpoint::DOWN);

		m_targetPoints = 7;
	}

	void Grid::Level3() {

		Engine::Vector2i vec = Engine::Vector2i::Invalid();
		std::vector<Block*> blocks;

		blocks.push_back(new Block(vec, nullptr, 1, Block::HOSAKA, Block::NONE, { 0, 1 , 0, -1 }));
		blocks.push_back(new Block(vec, nullptr, 3, Block::HOSAKA, Block::NONE, { 0, 1 , 0, -1 }));
		blocks.push_back(new Block(vec, nullptr, 2, Block::MAAS_BIOLABS, Block::NONE, { 0, 1 , 0, -1 }));
		blocks.push_back(new Block(vec, nullptr, 0, Block::PORTAL, Block::NONE, { 0, 1 , 0, -1 }));
		blocks.push_back(new Block(vec, nullptr, 0, Block::PORTAL, Block::NONE, { 0, 1 , 0, -1 }));
		PlaceBlocks(blocks);

		m_startPoint = CreateEndpoint({ 0, 2 }, Endpoint::START, Endpoint::RIGHT);
		m_endPoint = CreateEndpoint({ 4, 2 }, Endpoint::END, Endpoint::RIGHT);

		m_targetPoints = 9;
	}

	void Grid::Level4() {

		Engine::Vector2i vec = Engine::Vector2i::Invalid();
		std::vector<Block*> blocks;

		blocks.push_back(new Block(vec, nullptr, 3, Block::HOSAKA, Block::NONE, { 0, 1 , 0, -1 }));
		blocks.push_back(new Block(vec, nullptr, 1, Block::HOSAKA, Block::NONE, { 0, 1 , 0, -1 }));
		blocks.push_back(new Block(vec, nullptr, 0, Block::PORTAL, Block::NONE, { 1, 0 , 0, -1 }));
		blocks.push_back(new Block(vec, nullptr, 2, Block::MAAS_BIOLABS, Block::NONE, { 0, 1 , -1, 0 }));
		blocks.push_back(new Block(vec, nullptr, 2, Block::MAAS_BIOLABS, Block::NONE, { 0, 1 , 0, -1 }));
		blocks.push_back(new Block(vec, nullptr, 1, Block::ZAIBATSU, Block::MAAS_BIOLABS, { 0, 1 , 0, -1 }));
		blocks.push_back(new Block(vec, nullptr, 2, Block::ZAIBATSU, Block::MAAS_BIOLABS, { 0, 1 , 0, -1 }));
		blocks.push_back(new Block(vec, nullptr, 0, Block::PORTAL, Block::NONE, { 0, 1 , 0, -1 }));
		PlaceBlocks(blocks);

		m_startPoint = CreateEndpoint({ 0, 1 }, Endpoint::START, Endpoint::RIGHT);
		m_endPoint = CreateEndpoint({ 4, 3 }, Endpoint::END, Endpoint::RIGHT);

		m_targetPoints = 16;
	}

	void Grid::Level5() {

		Engine::Vector2i vec = Engine::Vector2i::Invalid();
		std::vector<Block*> blocks;

		blocks.push_back(new Block(vec, nullptr, 1, Block::ZAIBATSU, Block::MAAS_BIOLABS, { 0, 1 , 0, -1 }));
		blocks.push_back(new Block(vec, nullptr, 1, Block::HOSAKA, Block::NONE, { 0, 0 , 1, -1 }));
		blocks.push_back(new Block(vec, nullptr, 2, Block::ZAIBATSU, Block::HOSAKA, { -1, 0 , 0, 1 }));
		blocks.push_back(new Block(vec, nullptr, 1, Block::MAAS_BIOLABS, Block::NONE, { 0, 1 , 0, -1 }));
		PlaceBlocks(blocks);

		m_startPoint = CreateEndpoint({ 0, 2 }, Endpoint::START, Endpoint::RIGHT);
		m_endPoint = CreateEndpoint({ 0, 1 }, Endpoint::END, Endpoint::LEFT);

		m_targetPoints = 4;
	}

	void Grid::Level6() {

		Engine::Vector2i vec = Engine::Vector2i::Invalid();
		std::vector<Block*> blocks;

		blocks.push_back(new Block(vec, nullptr, 5, Block::HOSAKA, Block::NONE, { 0, 1 , -1, 0 }));
		blocks.push_back(new Block(vec, nullptr, 4, Block::HOSAKA, Block::NONE, { 0, 1 , -1, 0 }));
		blocks.push_back(new Block(vec, nullptr, 0, Block::PORTAL, Block::NONE, { 1, 0 , 0, -1 }));
		blocks.push_back(new Block(vec, nullptr, 0, Block::PORTAL, Block::NONE, { 1, 0 , 0, -1 }));
		blocks.push_back(new Block(vec, nullptr, 0, Block::PORTAL, Block::NONE, { 0, 1 , -1, 0 }));
		blocks.push_back(new Block(vec, nullptr, 5, Block::ZAIBATSU, Block::HOSAKA, { 1, 0 , -1, 0 }));
		PlaceBlocks(blocks);

		m_startPoint = CreateEndpoint({ 0, 2 }, Endpoint::START, Endpoint::RIGHT);
		m_endPoint = CreateEndpoint({ 1, 4 }, Endpoint::END, Endpoint::UP);

		m_targetPoints = 15;
	}

	void Grid::Level7() {

		Engine::Vector2i vec = Engine::Vector2i::Invalid();
		std::vector<Block*> blocks;

		blocks.push_back(new Block(vec, nullptr, 0, Block::PORTAL, Block::NONE, { 0, 1 , 0, -1 }));
		blocks.push_back(new Block(vec, nullptr, 1, Block::HOSAKA, Block::NONE, { 1, 0 , 0, -1 }));
		blocks.push_back(new Block(vec, nullptr, 2, Block::MAAS_BIOLABS, Block::NONE, { 1, 0 , -1, 0 }));
		blocks.push_back(new Block(vec, nullptr, 0, Block::PORTAL, Block::NONE, { 0, 1 , -1, 0 }));
		blocks.push_back(new Block(vec, nullptr, 1, Block::ZAIBATSU, Block::MAAS_BIOLABS, { 0, 0 , 1, -1 }));
		blocks.push_back(new Block(vec, nullptr, 2, Block::ZAIBATSU, Block::HOSAKA, { -1, 1 , 0, 0 }));
		blocks.push_back(new Block(vec, nullptr, 0, Block::PORTAL, Block::NONE, { 0, 1 , 0, -1 }));
		blocks.push_back(new Block(vec, nullptr, 5, Block::ZAIBATSU, Block::ZAIBATSU, { 0, 1 , 0, -1 }));
		PlaceBlocks(blocks);

		m_startPoint = CreateEndpoint({ 0, 1 }, Endpoint::START, Endpoint::RIGHT);
		m_endPoint = CreateEndpoint({ 4, 2 }, Endpoint::END, Endpoint::RIGHT);

		m_targetPoints = 38;
	}

	void Grid::Level8() {

		Engine::Vector2i vec = Engine::Vector2i::Invalid();
		std::vector<Block*> blocks;

		blocks.push_back(new Block(vec, nullptr, 0, Block::PORTAL, Block::NONE, { 0, -1 , 1, 0 }));
		blocks.push_back(new Block(vec, nullptr, 5, Block::ZAIBATSU, Block::NONE, { -1, 0 , 0, 1 }));
		blocks.push_back(new Block(vec, nullptr, 5, Block::ZAIBATSU, Block::NONE, { 0, -1 , 0, 1 }));
		blocks.push_back(new Block(vec, nullptr, 5, Block::ZAIBATSU, Block::NONE, { 0, -1 , 1, 0 }));
		blocks.push_back(new Block(vec, nullptr, 0, Block::PORTAL, Block::NONE, { -1, 0 , 1, 0 }));
		blocks.push_back(new Block(vec, nullptr, 5, Block::ZAIBATSU, Block::ZAIBATSU, { -1, 0 , 0, 1 }));
		blocks.push_back(new Block(vec, nullptr, 5, Block::ZAIBATSU, Block::ZAIBATSU, { 0, -1, 0, 1 }));
		blocks.push_back(new Block(vec, nullptr, 5, Block::ZAIBATSU, Block::ZAIBATSU, { 0, -1, 1, 0 }));
		blocks.push_back(new Block(vec, nullptr, 0, Block::PORTAL, Block::NONE, { -1, 0 , 0, 1 }));
		PlaceBlocks(blocks);

		m_startPoint = CreateEndpoint({ 4, 4 }, Endpoint::START, Endpoint::LEFT);
		m_endPoint = CreateEndpoint({ 0, 0 }, Endpoint::END, Endpoint::LEFT);

		m_targetPoints = 285;
	}
}