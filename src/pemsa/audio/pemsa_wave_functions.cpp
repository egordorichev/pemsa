#include "pemsa/audio/pemsa_wave_functions.hpp"
#include "pemsa/audio/pemsa_audio_module.hpp"
#include "pemsa/util/pemsa_util.hpp"

#include <cmath>
#include <chrono>
#include <random>

double pemsa_sine(double t) {
	return sin(M_PI_2 * t * 4);
}

double pemsa_square(double t) {
	return pemsa_sine(t) >= 0 ? 1.0 : -1.0;
}

double pemsa_pulse(double t) {
	return (fmod(t, 1) < 0.3125 ? 1 : -1);
}

double pemsa_tilted_saw(double t) {
	t = fmod(t, 1);
	return (((t < 0.875) ? (t * 16 / 7) : ((1 - t) * 16)) - 1) * 0.7;
}

double pemsa_saw_tooth(double t) {
	return 2 * (t - (int) (t + 0.5));
}

double pemsa_organ(double t) {
	t *= 4;
	return (fabs(fmod(t, 2) - 1) - 0.5 + (fabs(fmod(t * 0.5, 2) - 1) - 0.5) / 2.0 - 0.1);
}

double pemsa_phaser(double t) {
	t *= 2;
	return (fabs(fmod(t, 2) - 1) - 0.5f + (fabs(fmod((t * 127 / 128), 2) - 1) - 0.5) / 2) - 0.25;
}

static std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
static double lastT = 0;
static double sample = 0;
static double tScale = NOTE_TO_FREQUENCY(63) / PEMSA_SAMPLE_RATE;

double pemsa_noise(double t) {
	double scale = (t - lastT) / tScale;
	double lastSample = sample;

	sample = (lastSample + scale * ((rng() / (double) rng.max()) * 2 - 1)) / (1.0 + scale);
	lastT = t;

	return fmin(fmax((lastSample + sample) * 4.0 / 3.0 * (1.75 - scale), -1), 1) * 0.7f;
}

static PemsaWaveFn wave_functions[] = {
	pemsa_sine, pemsa_tilted_saw,
	pemsa_saw_tooth, pemsa_square,
	pemsa_pulse, pemsa_organ,
	pemsa_noise, pemsa_phaser
};

double pemsa_sample(int function, double t) {
	if (function < 0 || function > 7) {
		return 0;
	}

	return wave_functions[function](t);
}
