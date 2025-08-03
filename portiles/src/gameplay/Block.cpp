#include "Block.h"
#include "nlohmann/json.hpp"
#include <fstream>
#include <random>

#include "engine/core/Log.h"
#include <engine.h>
#include "Grid.h"

namespace game1 {
	
	Block::Block(Engine::Vector2i pos, Tile* tile, int value, BlockType type, BlockType typeBonus, std::array<int, 4> connections)
		: m_pos(pos),
		m_tile(tile),
		m_value(value),
		m_type(type),
		m_typeBonus(typeBonus),
		m_connections(connections),
		m_activated(false) 
	{
		m_ScoringRotation.StartInterpolation(180.f, 180.f, 0.f);
		m_ScoringScale.StartInterpolation(1.f, 1.f, 0.f);

		m_FontScallingScoring.StartInterpolation(1.f, 1.f, 0.f);
	}

	void Block::Update() {

	}

	void Block::Activate(int* currentPoints, std::map<Block::BlockType, std::vector<Block*>>* blocksActivated, Grid* grid) {
		m_activated = true;

		int pointsScored = 0;
		if (m_typeBonus == NONE) {
			pointsScored = m_value;
			grid->AddPointsText(m_value, this);
		}
		else {
			if (blocksActivated->find(m_typeBonus) == blocksActivated->end()) {
				blocksActivated[m_typeBonus];
			}
			pointsScored = m_value * (*blocksActivated)[m_typeBonus].size();

			grid->AddPointsText(m_value, &(*blocksActivated)[m_typeBonus]);
		}

		*currentPoints += pointsScored;
		(*blocksActivated)[m_type].push_back(this);

		if (pointsScored > 0) {
			float volume = m_typeBonus != NONE ? 0.8f : 0.5f;
			Engine::Application::getApplication()->getAudioPlayer()->PlaySound("assets/audio/score.wav", false, 0.3);
		}
	}

	int Block::GetOutDirection() {
		for (int i = 0; i < 4; i++) {
			if (m_connections[i] == 1) {
				return i;
			}
		}
	}
	void Block::Rotate() {
		int last = m_connections[3];
		for (int i = 3; i >= 1; i--) {
			m_connections[i] = m_connections[i - 1];
		}
		m_connections[0] = last;
	}

	bool Block::Hovering(Engine::Vector2i mouseGamePos) { // Probably could store a rectangle or something, but I don't want to right now
		return mouseGamePos.x >= m_pos.x - m_size / 2 && mouseGamePos.x < m_pos.x + m_size / 2 && mouseGamePos.y >= m_pos.y - m_size / 2 && mouseGamePos.y < m_pos.y + m_size / 2;
	}

	void Block::Draw(bool highlight) {
		glm::vec4 tint = { 1,1,1,1 };
		if (highlight) {
			tint = { 0.6f, 0.6f, 0.6f, 1 };
		}

		Engine::Renderer2D::DrawQuad({ m_pos.x, m_pos.y, 0.81  }, { m_size * m_ScoringScale.QuadraticEaseOut(), m_size * m_ScoringScale.QuadraticEaseOut() }, m_ScoringRotation.QuadraticEaseIn(), m_typeTextures[m_type], tint);
		if (m_type != PORTAL) {
			Engine::Renderer2D::DrawQuad({ m_pos.x, m_pos.y, 0.82 }, { m_size * m_ScoringScale.QuadraticEaseOut(), m_size * m_ScoringScale.QuadraticEaseOut()}, m_ScoringRotation.QuadraticEaseIn(), m_numberTextures[m_value][m_typeBonus], tint);
		}

		for (int i = 0; i < m_connections.size(); i++) {
			if (m_connections[i] != 0) {
				Engine::Renderer2D::DrawQuad({ m_pos.x, m_pos.y, 0.83 + (i * 0.001f) }, { m_size,m_size }, m_arrowTextures[i][m_connections[i]]);
			}
		}
	}
	void Block::DrawOutline() {
		Engine::Renderer2D::DrawQuad({ m_pos.x, m_pos.y, 0.88 }, { m_size, m_size }, m_outlineTex);
	}

	void Block::PlayScoreAnimation()
	{
		if (m_ScoringRotation.isAnimationRunning)
			return;

		// It's a feature not a bug
		// When interpolating between two of the same non zero points, non-linear interpolation's output approchs 0 before bouncing back to the input
		m_ScoringRotation.StartInterpolation(180.f, 180.f, 1.f);
		m_ScoringScale.StartInterpolation(1.f, 1.f, 1.f);
		m_FontScallingScoring.StartInterpolation(1.f, 1.f, 1.f);
	}

	void Block::LoadBlockData() {
		return;
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

		// Outline
		m_outlineTex = Engine::Texture2D::Create("assets/textures/grid/outline.png");
	}
}