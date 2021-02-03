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
#include "Generator.h"

#include <chrono>

int main(int, char **)
{
	std::cout << "Program start" << std::endl;
	SDL_Init(SDL_INIT_EVERYTHING);
	try {

		AudioDriver adrv;
		SineWaveGenerator		gen1;
		SquareWaveGenerator		gen2;
		SawtoothWaveGenerator	gen3;
		ADSRGenerator			adsr	{ 0., 20., 0.95, 1.0, 10. };

		double master_volume = 5000.;

		gen1.set_frequency(440.);
		gen2.set_frequency(440.);
		gen3.set_frequency(440.);

		size_t sound_fragment_size =
				AudioDriver::SAMPLING_FREQ * 3; // 3 секунды
		short sound_fragment[sound_fragment_size];
		for (unsigned i = 0; i < sound_fragment_size; ++i) {
			if ((i % AudioDriver::SAMPLING_FREQ) == 0)
				adsr.on();
			if (i < AudioDriver::SAMPLING_FREQ)
				sound_fragment[i] = short( master_volume * adsr.sample() *
						gen1.sample() );
			else if (i < AudioDriver::SAMPLING_FREQ * 2)
				sound_fragment[i] = short( master_volume * adsr.sample() *
						gen2.sample() );
			else
				sound_fragment[i] = short( master_volume * adsr.sample() *
						gen3.sample() );


		}
		adrv.enqueue(sound_fragment, sound_fragment_size);

		using clk = std::chrono::high_resolution_clock;
		auto tp1 = clk::now();
		auto tp2 = tp1 + std::chrono::seconds(3);

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
