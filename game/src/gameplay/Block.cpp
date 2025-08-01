#include "Block.h"
#include "nlohmann/json.hpp"
#include <fstream>
#include <random>

#include "engine/core/Log.h"
#include <engine.h>

namespace game1 {
	
	Block::Block(int value, BlockType type, BlockType typeBonus)
		: m_value(value),
		m_type(type),
		m_typeBonus(typeBonus) {}

	void Block::Draw(int x, int y) {
		Engine::Renderer2D::DrawQuad(glm::vec3(x, y, 1), { 32, 32 }, m_typeTextures[m_type]);
		Engine::Renderer2D::DrawQuad(glm::vec3(x, y, 1), { 32, 32 }, m_numberTextures[m_value][m_typeBonus]);
	}

	Block* Block::GenerateBlock() {
		return new Block(3, HOSAKA, NONE);
	}

	void Block::LoadBlockData() {

		// Load JSON data
		std::ifstream infile{ "data/blocks.json" };
		std::string contents{ std::istreambuf_iterator<char>(infile), std::istreambuf_iterator<char>() };
		nlohmann::json data;
		try {
			data = nlohmann::json::parse(contents);
		}
		catch (const nlohmann::json::parse_error& e){
			EG_TRACE("JSON Error while reading data/blocks.json: {0}", e.what());
		}

		int i = 0;
		for (const auto& manufacturer : data["manufacturers"]) {
			m_types[i++] = manufacturer.get<std::string>();
		}
	}

	void Block::LoadBlockTextures() {
		Engine::Ref<Engine::Texture2D> tex = Engine::Texture2D::Create("assets/textures/grid/blocks.png");
		
		// Type textures
		for (int i = 0; i < 4; i++) {
			m_typeTextures[(BlockType)i] = Engine::SubTexture2D::CreateFromCoords(tex, { i, 4 }, { 32,32 });
		}

		// Numbers
		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 4; j++) {
				m_numberTextures[i + 1][(BlockType)(j - 1)] = Engine::SubTexture2D::CreateFromCoords(tex, { i, 3 - j }, { 32, 32 });
			}
		}
	}
}