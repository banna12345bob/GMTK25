#pragma once

#include <string>
#include <vector>
#include <map>

namespace game1 {
	
	class Block
	{
	public:

		enum Manufacturer {
			HOSAKA = 0,
			MAAS_BIOLABS = 1
		};

		struct Modifier {
			std::string type;
			int amount;
			int rarity;

			// Example: 'manufacturer:Hosaka'. This is just temporary though
			std::string condition;

			Modifier(std::string t, int a, int r, std::string c)
				: type(std::move(t)),
				amount(a),
				rarity(r),
				condition(std::move(c)) {}
		};

		struct BaseBlock {
			std::string type;
			int value;
			int rarity;

			BaseBlock(std::string t, int v, int r)
				: type(std::move(t)),
				value(v),
				rarity(r) {}
		};

		Block::Block(BaseBlock baseBlock, Manufacturer manufacturer, Modifier modifier);

		static void LoadBlockData();

	private:
		BaseBlock m_baseBlock;

		// Only used for modifiers, does not do anything
		Manufacturer m_manufacturer;

		Modifier m_modifier;

		// Both are a map of lists, where rarity is the key
		static inline std::map<int, std::vector<BaseBlock>> m_baseBlocks;
		static inline std::map<int, std::vector<Modifier>> m_modifiers;

		static inline std::map<int, std::string> m_manufacturers;
	};
}


