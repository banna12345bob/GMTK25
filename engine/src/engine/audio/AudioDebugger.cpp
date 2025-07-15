#include "AudioDebugger.h"

#include <imgui.h>
#include <filesystem>

namespace Engine {

	AudioDebugger::AudioDebugger(Scope<AudioPlayer>* audioPlayer)
		: m_AudioPlayer(audioPlayer)
	{
	}

	void AudioDebugger::renderImGUILayer() {
		EG_PROFILE_FUNCTION();
		if (!this->m_ShowWindow)
			return;

		ImGui::Begin("Audio Debugger", &this->m_ShowWindow);
			if (ImGui::Button("Stop All")) {
				for (auto i = m_AudioPlayer->get()->GetSounds()->begin(); i != m_AudioPlayer->get()->GetSounds()->end(); i++) {
					m_AudioPlayer->get()->StopSound(i->first);
				}
			}
			for (auto i = m_AudioPlayer->get()->GetSounds()->begin(); i != m_AudioPlayer->get()->GetSounds()->end(); i++) {
				// Incase of edge case where threads don't align
				if (m_AudioPlayer->get()->GetSounds()->size() == 0)
					break;

				int id = i->first;
				if (ImGui::TreeNode(std::to_string(id).c_str())) {
					ImGui::Text(i->second->filePath.c_str());
					ImGui::SliderFloat("Volume", &i->second->volume, 0, 1);
					ImGui::Checkbox("Loop", &i->second->loop);
					if (ImGui::Button("Stop"))
						m_AudioPlayer->get()->StopSound(id);
					ImGui::TreePop();
				}
			}
		ImGui::End();

		ImGui::Begin("Audio Debug Player");
			ImGui::Checkbox("Loop", &temp_loop);
			ImGui::SliderFloat("Volume", &temp_volume, 0, 1);
			for (const auto& entry : std::filesystem::directory_iterator("assets/audio/music")) {
				if (ImGui::Button(entry.path().string().c_str()))
					m_AudioPlayer->get()->PlaySound(entry.path().string(), temp_loop, temp_volume);
			}
		ImGui::End();
	}

}