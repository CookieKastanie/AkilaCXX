#pragma once

#include <miniaudio/miniaudio.h>
#include <string>

namespace akila {
	class AudioBuffer {
	public:
		AudioBuffer();
		~AudioBuffer();

		void loadFromFile(std::string const &path);

	private:
		friend class AudioEmitter;

		ma_resource_manager_data_source source;
	};
}
