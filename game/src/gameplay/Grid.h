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
		bool Start(std::string* errorMessage);
		bool CheckValidCircuit();
		void Update(Engine::Timestep deltaTime);
		void AddPointsText(int value, Block* block);
		void AddPointsText(int value, std::vector<Block*>* gridPositions);
		int GetTimesActivated(Block* block);
		Tile* GetTile(Engine::Vector2i position);
		Tile* GetTile(int x, int y);
		bool HoveringGrid();
		Engine::Vector2i GridPosToScreenPos(Engine::Vector2i tilePos);

		void Retry();
		void NextRound();

		void Draw();

	private:

		struct PointsText {

			int count; // times activated
			int points;
			glm::vec3 pos;
			int msLeft;

			PointsText() : PointsText(0, 0, { 0,0,0 }, 0) {}
			PointsText(int count, int points, glm::vec3 pos, int duration)
				: count(count),
				points(points),
				pos(pos),
				msLeft(duration) {}

			std::string GetString() { return "+" + std::to_string(points); }
		};

		struct Endpoint {

			enum Type {
				START = -1,
				END = 1
			};

			enum Direction {
				UP = 0,
				RIGHT = 1,
				DOWN = 2,
				LEFT = 3
			};

			Engine::Vector2i gridPos;
			glm::vec3 screenPos;
			Type type;
			Direction dir;

			Endpoint() : Endpoint({ -1,-1 }, { -1,-1,-1 }, START, UP) {}
			Endpoint(Engine::Vector2i gridPos, glm::vec3 screenPos, Type type, Direction dir)
				: gridPos(gridPos),
				screenPos(screenPos),
				type(type),
				dir(dir) {}
		};

		Engine::Vector2i GetNextBlockPos(Block* current);
		Block* GetHoveredBlock(Engine::Vector2i hoveredTile);
		void SwapBlocks(Block* b1, Block* b2);
		void MoveBlock(Block* block, Engine::Vector2i hoveredTile);
		Engine::Vector2i GetHoveredTile();
		void RemoveFromUnattachedList(Block* block);
		Endpoint CreateEndpoint(Engine::Vector2i gridPos, Endpoint::Type type, Endpoint::Direction side);
		void Complete();

		GameLayer* m_gameLayer;

		int m_size;
		Engine::Vector2i m_gridOffset; // To center it
		Engine::Ref<Engine::Texture2D> m_emptyTileTex;
		std::vector<std::vector<Tile>> m_tiles;

		int m_currentPuzzle;

		bool m_executing;
		Engine::Vector2i m_currentPos;
		Block* m_currentBlock;
		int m_msToActivateBlock;	// Time before moving onto the next block
		int m_msActivating;			// Exectuting the current block
		std::map<Block::BlockType, std::vector<Block*>> m_blocksActivated;

		int m_targetPoints;
		int m_currentPoints;
		TextRendering* m_textRenderer;
		std::map<Block*, PointsText> m_pointsText;
		int m_pointsTextDuration;

		Block* m_selectedBlock;
		Engine::Ref<Engine::Texture2D> m_outlineTex;

		std::vector<Block*> m_unattachedBlocks;

		Endpoint m_startPoint;
		Endpoint m_endPoint;
		std::vector<Engine::Ref<Engine::Texture2D>> m_endpointArrowTexs;

		Engine::Ref<Engine::Texture2D> m_borderTex;

		Engine::Vector2i m_lastPortalPos;
		bool m_teleportNextTurn;
		
		static inline int m_cellSize = 32;
	};
}