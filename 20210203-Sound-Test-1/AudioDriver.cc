/*
 * AudioDriver.cc
 *
 *  Created on: 3 февр. 2021 г.
 *      Author: unyuu
 */

#include <stdexcept>
#include <string>

#include "AudioDriver.h"

AudioDriver::AudioDriver()
{
	using namespace std::literals;

	_spec.freq = SAMPLING_FREQ;
	_spec.channels = 1;
	_spec.format = AUDIO_S16;
	_spec.samples = 512;

	_dev = SDL_OpenAudioDevice(nullptr, 0, &_spec, &_spec, 0);
	if (_dev < 2)
		throw std::runtime_error(
				"Не могу открыть аудиоустройство: "s +
				SDL_GetError());

	unpause(); // Возможно, этого делать не нужно, но пока так.
}


AudioDriver::~AudioDriver()
{
	pause();
	SDL_CloseAudioDevice(_dev);
}

