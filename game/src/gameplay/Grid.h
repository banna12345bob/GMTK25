#pragma once

#include "engine.h"
#include <vector>
#include "Block.h"
#include "UI/TextRendering.h"

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
		void Update(Engine::Timestep deltaTime);
		void ActivateBlock(Block* block);
		void AddPointsText(int value, Engine::Vector2i gridPos);
		void AddPointsText(int value, std::vector<Engine::Vector2i>* gridPositions);
		Tile* GetTile(Engine::Vector2i position);
		Tile* GetTile(int x, int y);

		void Draw();

	private:

		struct PointsText {

			std::string text;
			glm::vec3 pos;
			int msLeft;

			PointsText(std::string text, glm::vec3 pos)
				: text(text),
				pos(pos),
				msLeft(m_pointsTextDuration) {}
		};

		Engine::Ref<Engine::Texture2D> m_emptyTileTex;
		int m_size;
		std::vector<std::vector<Tile>> m_tiles;

		bool m_executing;
		Engine::Vector2i m_currentPos;
		Block* m_currentBlock;
		int m_msToActivateBlock;	// Time before moving onto the next block
		int m_msActivating;			// Exectuting the current block
		std::map<Block::BlockType, std::vector<Engine::Vector2i>> m_blocksActivated;

		int m_currentPoints;
		TextRendering* m_textRenderer;
		std::vector<PointsText> m_pointsText;
		static inline int m_pointsTextDuration = 400;
		
		static inline int m_cellSize = 32;
	};
}