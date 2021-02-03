/*
 * AudioDriver.h
 *
 *  Created on: 3 февр. 2021 г.
 *      Author: unyuu
 */

#ifndef AUDIODRIVER_H_
#define AUDIODRIVER_H_

#include <SDL2/SDL.h>

class AudioDriver
{
public:
	static constexpr int SAMPLING_FREQ = 48000;

protected:
	SDL_AudioSpec _spec {};
	SDL_AudioDeviceID _dev {};

public:
	AudioDriver();
	virtual ~AudioDriver();

	inline void pause()   { SDL_PauseAudioDevice(_dev, 1); }
	inline void unpause() { SDL_PauseAudioDevice(_dev, 0); }

	void enqueue(const short * data, size_t length)
	{
		SDL_QueueAudio(_dev, data, length * sizeof(short));
	}
};

#endif /* AUDIODRIVER_H_ */
