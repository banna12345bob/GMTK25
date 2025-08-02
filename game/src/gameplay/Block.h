#pragma once

#include <string>
#include <vector>
#include <map>

#include "engine.h"

namespace game1 {
	
	class Grid; // Forward declare to avoid cicular dependency.
	struct Tile;

	class Block
	{
	public:

		enum BlockType {
			NONE = -1,
			HOSAKA = 0,
			MAAS_BIOLABS = 1,
			ZAIBATSU = 2,
			PORTAL = 3
		};

		Block(Engine::Vector2i pos, Tile* tile, int value, BlockType type, BlockType typeBonus, std::array<int, 4> connections);
		
		void Update();
		void Activate(int* currentPoints, std::map<Block::BlockType, std::vector<Block*>>* blocksActivated, Grid* grid);
		int GetOutDirection();
		void Rotate();
		bool Hovering(Engine::Vector2i mouseGamePos);
		void Draw(bool activating);
		void DrawOutline();

		Engine::Vector2i getPos() const { return m_pos; }
		void setPos(Engine::Vector2i pos) { m_pos = pos; }
		Tile* getTile() const { return m_tile; }
		void setTile(Tile* tile) { m_tile = tile; }


		static void LoadBlockData();
		static void LoadBlockTextures();

	private:
		Engine::Vector2i m_pos;
		Tile* m_tile;
		int m_value;
		BlockType m_type;
		BlockType m_typeBonus; // Receives points equal to value for each of this type that has been activated
		std::array<int, 4> m_connections; // Top, right, bottom, left

		static inline int m_size = 32;

		// Both are a map of lists, where rarity is the key
		static inline std::map<int, std::string> m_types;

		// Textures
		static inline std::map<BlockType, Engine::Ref<Engine::Texture2D>> m_typeTextures;
		static inline std::map<int, std::map<BlockType, Engine::Ref<Engine::Texture2D>>> m_numberTextures;
		static inline std::vector<std::map<int, Engine::Ref<Engine::Texture2D>>> m_arrowTextures = std::vector<std::map<int, Engine::Ref<Engine::Texture2D>>>(4);
		static inline Engine::Ref<Engine::Texture2D> m_outlineTex;
	};
}