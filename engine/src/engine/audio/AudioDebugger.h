#pragma once

#include "engine/audio/audioPlayer.h"
#include "engine/debug/Instrumentor.h"
#include "engine/graphics/imgui/ImGuiLayer.h"

namespace Engine {

	class AudioDebugger : public ImGuiLayer
	{
	public:
		AudioDebugger();

		virtual void renderImGUILayer() override;

	private:
		float temp_volume = 0.5f;
		bool temp_loop = false;
	};

}
