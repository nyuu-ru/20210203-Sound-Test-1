/*
 * main.cc
 *
 *  Created on: 3 февр. 2021 г.
 *      Author: unyuu
 */

#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_main.h>

#include "AudioDriver.h"

#include <cmath>
#include <numbers>
#include <chrono>

int main(int, char **)
{
	std::cout << "Program start" << std::endl;
	SDL_Init(SDL_INIT_EVERYTHING);
	try {

		AudioDriver adrv;

		size_t sound_fragment_size =
				AudioDriver::SAMPLING_FREQ * 2; // 2 секунды
		short sound_fragment[sound_fragment_size];
		for (unsigned i = 0; i < sound_fragment_size; ++i) {
			double time = 	double(i) /
							double(AudioDriver::SAMPLING_FREQ);
			sound_fragment[i] = short( 5000 *
					sin(time * 2. * std::numbers::pi * 880.)
					);
		}
		adrv.enqueue(sound_fragment, sound_fragment_size);

		using clk = std::chrono::high_resolution_clock;
		auto tp1 = clk::now();
		auto tp2 = tp1 + std::chrono::seconds(2);

		while (clk::now() < tp2)
			; // ничего не делаем


	} catch (const std::exception & e) {
		std::cerr << "Произошла ошибка:\n" <<
				e.what() << std::endl;
		return 1;
	} catch (...) {
		std::cerr << "Произошла неизвестная ошибка!" << std::endl;
		return 1;
	}
	std::cout << "Program end" << std::endl;
	return 0;
}
