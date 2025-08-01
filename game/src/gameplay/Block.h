#pragma once

#include <string>
#include <vector>
#include <map>

#include "engine.h"

namespace game1 {
	
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

		Block(int value, BlockType type, BlockType typeBonus, std::array<int, 4> connections);
		
		void Update();
		void Activate(int* currentPoints, std::map<Block::BlockType, std::vector<Engine::Vector2i>>* blocksActivated, Engine::Vector2i pos);
		int GetOutDirection();
		void Draw(int x, int y, bool activating);

		static void LoadBlockData();
		static void LoadBlockTextures();
		static Block* GenerateBlock();

	private:
		int m_value;
		BlockType m_type;
		BlockType m_typeBonus; // Receives points equal to value for each of this type that has been activated
		std::array<int, 4> m_connections;

		// Both are a map of lists, where rarity is the key
		static inline std::map<int, std::string> m_types;

		// Textures
		static inline std::map<BlockType, Engine::Ref<Engine::Texture2D>> m_typeTextures;
		static inline std::map<int, std::map<BlockType, Engine::Ref<Engine::Texture2D>>> m_numberTextures;
		static inline std::vector<std::map<int, Engine::Ref<Engine::Texture2D>>> m_arrowTextures = std::vector<std::map<int, Engine::Ref<Engine::Texture2D>>>(4);
	};
}