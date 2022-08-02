#include "akila/audio/audio.hpp"
#include <iostream>

using namespace akila;

ma_context Audio::context;
ma_resource_manager Audio::resourceManager;
ma_engine Audio::engine;
ma_device Audio::device;

void dataCallback(ma_device *pDevice, void *pOutput, const void *pInput, ma_uint32 frameCount) {
	(void)pInput;
	ma_engine_read_pcm_frames((ma_engine *)pDevice->pUserData, pOutput, frameCount, NULL);
}

void Audio::init() {
	ma_resource_manager_config resourceManagerConfig = ma_resource_manager_config_init();
	resourceManagerConfig.decodedFormat = ma_format_f32;
	resourceManagerConfig.decodedChannels = 0;
	resourceManagerConfig.decodedSampleRate = 48000;

	ma_result result = ma_resource_manager_init(&resourceManagerConfig, &resourceManager);
	if(result != MA_SUCCESS) {
		std::cerr << "Failed to initialize resource manager." << std::endl;
		return std::exit(1);
	}

	result = ma_context_init(NULL, 0, NULL, &context);
	if(result != MA_SUCCESS) {
		std::cerr << "Failed to initialize context." << std::endl;
		return std::exit(1);
	}
	
	ma_device_config deviceConfig;
	ma_engine_config engineConfig;

	deviceConfig = ma_device_config_init(ma_device_type_playback);
	//deviceConfig.playback.pDeviceID = ...; // ne rien mettre = default
	deviceConfig.playback.format = resourceManager.config.decodedFormat;
	deviceConfig.playback.channels = 0;
	deviceConfig.sampleRate = resourceManager.config.decodedSampleRate;
	deviceConfig.dataCallback = dataCallback;
	deviceConfig.pUserData = &engine;

	result = ma_device_init(&context, &deviceConfig, &device);
	if(result != MA_SUCCESS) {
		std::cerr << "Failed to initialize device." << std::endl;
		std::exit(1);
	}

	engineConfig = ma_engine_config_init();
	engineConfig.pDevice = &device;
	engineConfig.pResourceManager = &resourceManager;
	engineConfig.noAutoStart = MA_TRUE;

	result = ma_engine_init(&engineConfig, &engine);
	if(result != MA_SUCCESS) {
		std::cerr << "Failed to initialize engine" << std::endl;
		ma_device_uninit(&device);
		std::exit(1);
	}

	result = ma_engine_start(&engine);
		if(result != MA_SUCCESS) {
		std::cerr << "WARNING: Failed to start engine." << std::endl;
	}
}

void Audio::terminate() {
	ma_engine_uninit(&engine);
	ma_device_uninit(&device);
	ma_resource_manager_uninit(&resourceManager);
}
