#pragma once

#include "engine.h"
#include <vector>
#include "Block.h"
#include "Tile.h"
#include "UI/TextRendering.h"

class GameLayer;

namespace game1 {


	class Grid
	{
	public:
		Grid(int size, GameLayer* gameLayer);
		void Start();
		void Update(Engine::Timestep deltaTime);
		void AddPointsText(int value, Block* block);
		void AddPointsText(int value, std::vector<Block*>* gridPositions);
		Tile* GetTile(Engine::Vector2i position);
		Tile* GetTile(int x, int y);
		bool HoveringGrid();

		Engine::Vector2i TilePosToScreenPos(Engine::Vector2i tilePos);

		void Draw();

	private:

		struct PointsText {

			std::string text;
			glm::vec3 pos;
			int msLeft;

			PointsText(std::string text, glm::vec3 pos, int duration)
				: text(text),
				pos(pos),
				msLeft(duration) {}
		};

		void ActivateBlock(Block* block);
		Block* GetHoveredBlock(Engine::Vector2i hoveredTile);
		void SwapBlocks(Block* b1, Block* b2);
		void MoveBlock(Block* block, Engine::Vector2i hoveredTile);
		Engine::Vector2i GetHoveredTile();

		void RemoveFromUnattachedList(Block* block);

		GameLayer* m_gameLayer;

		int m_size;
		Engine::Vector2i m_gridOffset; // To center it
		Engine::Ref<Engine::Texture2D> m_emptyTileTex;
		std::vector<std::vector<Tile>> m_tiles;

		bool m_executing;
		Engine::Vector2i m_currentPos;
		Block* m_currentBlock;
		int m_msToActivateBlock;	// Time before moving onto the next block
		int m_msActivating;			// Exectuting the current block
		std::map<Block::BlockType, std::vector<Block*>> m_blocksActivated;

		int m_currentPoints;
		TextRendering* m_textRenderer;
		std::vector<PointsText> m_pointsText;
		int m_pointsTextDuration;

		Block* m_selectedBlock;
		Engine::Ref<Engine::Texture2D> m_outlineTex;

		std::vector<Block*> m_unattachedBlocks;
		
		static inline int m_cellSize = 32;
	};
}