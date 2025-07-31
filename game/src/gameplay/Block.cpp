#include "Block.h"
#include "nlohmann/json.hpp"
#include <fstream>
#include <random>

#include "engine/core/Log.h"

namespace game1 {
	
	Block::Block(BaseBlock baseBlock, Manufacturer manufacturer, Modifier modifier)
		: m_baseBlock(baseBlock),
		m_manufacturer(manufacturer),
		m_modifier(modifier) {}

	void Block::LoadBlockData() {
		std::ifstream infile{ "data/blocks.json" };
		std::string contents{ std::istreambuf_iterator<char>(infile), std::istreambuf_iterator<char>() };
		nlohmann::json data;
		try {
			data = nlohmann::json::parse(contents);
		}
		catch (const nlohmann::json::parse_error& e){
			EG_TRACE("JSON Error while reading data/blocks.json: {0}", e.what());
		}

		for (const auto& blockData : data["baseBlocks"]) {
			int rarity = blockData[2];
			BaseBlock block = BaseBlock(blockData[0], blockData[1], rarity);

			m_baseBlocks[rarity].push_back(block);
		}

		for (const auto& modifierData : data["modifiers"]) {
			int rarity = modifierData[2];
			Modifier modifier = Modifier(modifierData[0], modifierData[1], rarity, modifierData[3]);

			m_modifiers[rarity].push_back(modifier);
		}

		int i = 0;
		for (const auto& manufacturer : data["manufacturers"]) {
			m_manufacturers[i++] = manufacturer.get<std::string>();
		}
	}
}