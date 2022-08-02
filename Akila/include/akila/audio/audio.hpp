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


		static ma_context context;
		static ma_resource_manager resourceManager;
		static ma_engine engine;
		static ma_device device;

		//static ma_device_info *pPlaybackDeviceInfos;
		//ma_uint32 playbackDeviceCount;

		static void init();
		static void terminate();
	};
}
