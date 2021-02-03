/*
 * Generator.h
 *
 *  Created on: 3 февр. 2021 г.
 *      Author: unyuu
 */

#ifndef GENERATOR_H_
#define GENERATOR_H_

#include <cmath>
#include <numbers>

#include "AudioDriver.h"

class Generator
{
protected:
	unsigned _current_sample;
public:
	virtual ~Generator() = default;

	inline unsigned current_sample() const { return _current_sample; }
	inline double current_time() const
	{
		return 	double(_current_sample) /
				AudioDriver::SAMPLING_FREQ;
	}

	virtual double sample() { ++_current_sample; return 0.; }
};

class WaveformGenerator: public Generator
{
protected:
	double _frequency {};
public:
	virtual ~WaveformGenerator() = default;

	void set_frequency(double freq)
	{
		if (freq >= 0.) _frequency = freq;
	}

	inline double frequency() const { return _frequency; }
	inline double omega() const { return 	_frequency * 2. *
											std::numbers::pi; }
};

class SineWaveGenerator: public WaveformGenerator
{
public:
	virtual ~SineWaveGenerator() = default;

	virtual double sample() override
	{
		double t = current_time();
		double result = sin( t * omega() );
		Generator::sample();
		return result;
	}
};

class SquareWaveGenerator: public WaveformGenerator
{
public:
	virtual ~SquareWaveGenerator() = default;

	virtual double sample() override
	{
		double t = current_time();
		double result = (sin( t * omega() ) > 0.) ? 1. : -1.;
		Generator::sample();
		return result;
	}
};

class SawtoothWaveGenerator: public WaveformGenerator
{
public:
	virtual ~SawtoothWaveGenerator() = default;

	virtual double sample() override
	{
		double t = current_time();
		double p = t * frequency();
		p = p - floor(p);
		double result = 1. - 2. * p;
		Generator::sample();
		return result;
	}
};

class ADSRGenerator: public Generator
{
protected:
	double _at, _dr, _sl, _sr, _rr;
	double _level { 0. };

	enum class State { IDLE, ATTACK, DECAY, SUSTAIN, RELEASE };
	State _state { State::IDLE };

public:
	ADSRGenerator(	double at, double dr, double sl,
					double sr, double rr)
	: _at(at), _dr(dr), _sl(sl), _sr(sr), _rr(rr)
	{}

	void on() { _level = 0.; _state = State::ATTACK; }
	void off()
	{
		if (_state != State::IDLE) _state = State::RELEASE;
	}

	virtual double sample() override
	{
		switch (_state) {
		case State::IDLE:
			break;
		case State::ATTACK:
			_level += 1. / AudioDriver::SAMPLING_FREQ / _at;
			if (_level >= 1.) {
				_level = 1.;
				_state = State::DECAY;
			}
			break;
		case State::DECAY:
			_level -= _dr / AudioDriver::SAMPLING_FREQ;
			if (_level <= _sl) {
				_level = _sl;
				_state = State::SUSTAIN;
			}
			break;
		case State::SUSTAIN:
			_level -= _sr / AudioDriver::SAMPLING_FREQ;
			if (_level <= 0.) {
				_level = 0.;
				_state = State::IDLE;
			}
			break;
		case State::RELEASE:
			_level -= _rr / AudioDriver::SAMPLING_FREQ;
			if (_level <= 0.) {
				_level = 0.;
				_state = State::IDLE;
			}
			break;
		}

		Generator::sample();
		return _level;
	}
};


#endif /* GENERATOR_H_ */
