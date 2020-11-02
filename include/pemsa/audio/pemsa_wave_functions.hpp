#ifndef PEMSA_WAVE_FUNCTIONS_HPP
#define PEMSA_WAVE_FUNCTIONS_HPP

typedef double (*PemsaWaveFn)(double t);

double pemsa_sine(double t);
double pemsa_square(double t);
double pemsa_pulse(double t);
double pemsa_tilted_saw(double t);
double pemsa_saw_tooth(double t);
double pemsa_organ(double t);
double pemsa_phaser(double t);
double pemsa_noise(double t);

double pemsa_sample(int function, double t);

#endif