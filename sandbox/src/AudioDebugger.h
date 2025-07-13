#pragma once

#include <engine.h>

#include <imgui/imgui.h>

class AudioDebugger : public Engine::ImGuiLayer
{
public:
	AudioDebugger(Engine::Scope<Engine::AudioPlayer>* audioPlayer)
		: m_AudioPlayer(audioPlayer)
	{
	}

	virtual void renderImGUILayer() override {
		EG_PROFILE_FUNCTION();
		if (!this->m_ShowWindow)
			return;
		ImGui::Begin("Audio Debugger", &this->m_ShowWindow);
		for (auto i = m_AudioPlayer->get()->GetSounds()->begin(); i != m_AudioPlayer->get()->GetSounds()->end(); i++) {
			// Incase of edge case where threads don't align
			if (m_AudioPlayer->get()->GetSounds()->size() == 0)
				break;

			int id = i->first;
			if (ImGui::TreeNode(std::to_string(id).c_str())) {
				ImGui::Text(i->second->filePath.c_str());
				ImGui::Text(("Volume: " + std::to_string(i->second->volume)).c_str());
				std::string loopValue = i->second->loop ? "True" : "False";
				ImGui::Text(("Loop: " + loopValue).c_str());
				if (ImGui::Button("Stop"))
					m_AudioPlayer->get()->StopSound(id);
				ImGui::TreePop();
			}
		}
		ImGui::End();
	}

private:
	Engine::Scope<Engine::AudioPlayer>* m_AudioPlayer;
};