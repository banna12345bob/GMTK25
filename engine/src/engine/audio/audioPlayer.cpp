#include "audioPlayer.h"

#include "platform/SDL/SDL_AudioPlayer.h"

namespace Engine {

	Scope<AudioPlayer> AudioPlayer::Create()
	{
		return Scope<SDL_AudioPlayer>();
	}
}