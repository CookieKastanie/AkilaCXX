#pragma once

#include <miniaudio/miniaudio.h>
#include <string>

namespace akila {
	class Audio {
	public:


	private:
		friend class Core;
		friend class AudioSource;
		friend class AudioEmitter;

		static ma_engine engine;

		static void init();
		static void terminate();
	};
}
