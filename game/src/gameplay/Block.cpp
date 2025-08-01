#include "Block.h"
#include "nlohmann/json.hpp"
#include <fstream>
#include <random>

#include "engine/core/Log.h"
#include <engine.h>

namespace game1 {
	
	Block::Block(int value, BlockType type, BlockType typeBonus, std::array<int, 4> connections)
		: m_value(value),
		m_type(type),
		m_typeBonus(typeBonus),
		m_connections(connections) {}

<<<<<<< HEAD
	void Block::Draw(int x, int y) {
		Engine::Renderer2D::DrawQuad(glm::vec3(x, y, .9f), { 32, 32 }, m_typeTextures[m_type]);
		Engine::Renderer2D::DrawQuad(glm::vec3(x, y, .9f), { 32, 32 }, m_numberTextures[m_value][m_typeBonus]);
=======
	void Block::Update() {

	}

	void Block::Activate(int* currentPoints, std::map<Block::BlockType, std::vector<Engine::Vector2i>>* blocksActivated, Engine::Vector2i pos) {
		if (m_typeBonus == NONE) {
			*currentPoints += m_value;
		}
		else {
			if (blocksActivated->find(m_typeBonus) == blocksActivated->end()) {
				blocksActivated[m_typeBonus];
			}
			*currentPoints += m_value * (*blocksActivated)[m_typeBonus].size();
		}

		(*blocksActivated)[m_type].push_back(pos);
	}

	int Block::GetOutDirection() {
		for (int i = 0; i < 4; i++) {
			if (m_connections[i] == 1) {
				return i;
			}
		}
	}

	void Block::Draw(int x, int y, bool activating) {
		glm::vec4 tint = { 1,1,1,1 };
		if (activating) {
			tint = { 0.6f, 0.7f, 0.9f, 1 };
		}
		Engine::Renderer2D::DrawQuad(glm::vec3(x, y, 0.81), { 32, 32 }, m_typeTextures[m_type], tint);
		Engine::Renderer2D::DrawQuad(glm::vec3(x, y, 0.82), { 32, 32 }, m_numberTextures[m_value][m_typeBonus], tint);

		for (int i = 0; i < m_connections.size(); i++) {
			if (m_connections[i] != 0) {
				Engine::Renderer2D::DrawQuad({ x, y, 0.83 + (i * 0.001f)}, {32,32}, m_arrowTextures[i][m_connections[i]]);
			}
		}
>>>>>>> 818a0ae (Draw blocks on grid and activate them)
	}

	Block* Block::GenerateBlock() {
		return new Block(3, HOSAKA, NONE, { 0, -1, 0, 1 });
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

		// Arrows
		tex = Engine::Texture2D::Create("assets/textures/grid/arrows.png");

		for (int i = 0; i < 4; i++) {
			m_arrowTextures[i][1] = Engine::SubTexture2D::CreateFromCoords(tex, { i, 1 }, { 32, 32 });	// Outgoing
			m_arrowTextures[i][-1] = Engine::SubTexture2D::CreateFromCoords(tex, { i, 0 }, { 32, 32 });	// Incoming
		}
	}
}